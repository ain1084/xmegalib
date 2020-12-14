/*
 * PCMDACAudioSampleRenderer.cpp
 *
 * Created: 2011/09/05 23:49:36
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include <string.h>
#include "Audio/ISampleGenerator.h"
#include "SystemClock.h"
#include "InterruptController.h"
#include "USARTInfo.h"
#include "PortInfo.h"
#include "TimerCounter1Info.h"
#include "DMAInfo.h"
#include "SerialAudioSampleRenderer.h"

namespace
{
	const int CHANNELS = 2;
	const int SAMPLE_BITS = 16;
}

using namespace XMEGA;
using namespace Audio;

SerialAudioSampleRenderer::SerialAudioSampleRenderer(USART_t& dataUSART,
 DMADoubleBufferMode dmaDoubleBufferMode,
 TC0_t& lrClockTimer, TC1_t& masterClockTimer, uint32_t samplingRate, uint16_t masterClockRatio, Format format /* =Format_I2S */)
 : _dataUSART(dataUSART)
 , _dmaDoubleBufferInfo(DMAInfo::GetDoubleBufferInfo(dmaDoubleBufferMode))
 , _lrClockTimerInfo(lrClockTimer)
 , _masterClockTimer(masterClockTimer)
 , _samplingRate(samplingRate)
 , _buffer(256)
 , _pGenerator(0)
 , _format(format)
 , _sampleCount(0)
 , _peakLevelL(0)
 , _peakLevelR(0)
 , _pause(false)
 , _running(false)
{
	USARTInfo dataUSARTInfo(dataUSART);
	TimerCounter0Info lrClockTimerInfo(lrClockTimer);
	TimerCounter1Info masterClockTimerInfo(masterClockTimer);
	
	const auto peripheralClock = SystemClock::GetInstance().GetPeripheralClock();
	
	CriticalSection cs;
	
	//SCLK = SAMPLING_RATE * SAMPLE_BITS * CHANNELS
	uint32_t sclkRate  = samplingRate * SAMPLE_BITS * CHANNELS;

	//usart baud = SCLK
	uint16_t usartBaud = (peripheralClock / sclkRate) / 2 - 1;
	
	//master clock
	uint16_t mclkCount;
	for (;;)
	{
		mclkCount = (peripheralClock / (samplingRate * masterClockRatio)) / 2;
		if (mclkCount != 0)
		{
			break;
		}
		masterClockRatio >>= 1;
	}
	_masterClockRatio = masterClockRatio;

	dataUSART.CTRLA = 0;
	dataUSART.CTRLB = USART_TXEN_bm;
	dataUSART.CTRLC = USART_CMODE_MSPI_gc;
	dataUSART.BAUDCTRLA = static_cast<uint8_t>((usartBaud >> 0) & 0xff);
	dataUSART.BAUDCTRLB = static_cast<uint8_t>((usartBaud >> 8) & 0xff);

	auto& dataPort = dataUSARTInfo.GetPort();

	//TXD0 (output)
	dataPort.DIRSET = dataUSARTInfo.GetTXDBitMask();

	//XCK0 (output, low)
	dataPort.DIRSET = dataUSARTInfo.GetXCKBitMask();
	dataPort.OUTCLR = dataUSARTInfo.GetXCKBitMask();

	//initialize event system
	EVSYS_CH0MUX = PortInfo(dataPort, dataUSARTInfo.GetXCKBitPosition()).GetEvent();

	//LRCLK (48KHz/44.1KHz)
	//
	//clock source is event channel 0
	//count start = 1 or 2 (I2S)
	//CCA = 31    16(bit) * 2(ch) - 1(start)
	lrClockTimer.CTRLA = TC_CLKSEL_EVCH0_gc;
	lrClockTimer.CTRLB = TC_WGMODE_FRQ_gc | TC1_CCAEN_bm;
	lrClockTimer.CTRLC = 0;
	lrClockTimer.CTRLD = 0;
	lrClockTimer.CNT = (format == Format_LeftJustified) ? 1 : 2;
	lrClockTimer.CCA = 31;
	auto& lrTimerPort = _lrClockTimerInfo.GetPort();
	lrTimerPort.DIRSET = _lrClockTimerInfo.GetOC0ABitMask();
	if (format == Format_LeftJustified)
	{
		//Start LEFT = High
		PortInfo(lrClockTimerInfo.GetPort(), lrClockTimerInfo.GetOC0ABitPosition()).PinCtrl() = PORT_INVEN_bm;
		lrClockTimerInfo.GetPort().OUTCLR = lrClockTimerInfo.GetOC0ABitMask();
	}
	else
	{
		//Start LEFT = Low
		PortInfo(lrClockTimerInfo.GetPort(), lrClockTimerInfo.GetOC0ABitPosition()).PinCtrl() = 0;
		lrClockTimerInfo.GetPort().OUTCLR = lrClockTimerInfo.GetOC0ABitMask();
	}

	//MCLK
	auto& masterClockTimerPort = masterClockTimerInfo.GetPort();
	masterClockTimer.CTRLA = TC_CLKSEL_DIV1_gc;
	masterClockTimer.CTRLB = TC_WGMODE_FRQ_gc | TC1_CCAEN_bm;
	masterClockTimer.CTRLC = 0;
	masterClockTimer.CTRLD = 0;
	masterClockTimer.CNT = 0;
	masterClockTimer.CCA = mclkCount - 1;
	PortInfo(masterClockTimerPort, masterClockTimerInfo.GetOC1ABitPosition()).PinCtrl() = PORT_INVEN_bm;
	masterClockTimerPort.OUTCLR = masterClockTimerInfo.GetOC1ABitMask();
	masterClockTimerPort.DIRSET = masterClockTimerInfo.GetOC1ABitMask();
	
	DMA_CTRL |= DMA_ENABLE_bm | _dmaDoubleBufferInfo.dbufMode;

	for (int i = 0; i < 2; ++i)
	{
		auto& channelInfo = _dmaDoubleBufferInfo.Channels[i];
		initDMA(channelInfo, dataUSARTInfo, _DMABuffer[i]);
		InterruptController::SetService(channelInfo.InterruptCause, dmaTransferCompleted, this);
	}

	InterruptController::SetService(_lrClockTimerInfo.GetOverflowInterruptCause(), timerOverflow, this);

	auto& firstChannelInfo = _dmaDoubleBufferInfo.Channels[0];
	#if defined(DMA_CH_CHEN_bm)
	firstChannelInfo.IO.CTRLA |= DMA_CH_CHEN_bm;
	#else
	firstChannelInfo.IO.CTRLA |= DMA_CH_ENABLE_bm;
	#endif
}

SerialAudioSampleRenderer::~SerialAudioSampleRenderer(void)
{
	CriticalSection cs;

	DMA_CTRL &=  ~_dmaDoubleBufferInfo.dbufMode;
	for (int i = 0; i < 2; ++i)
	{
		auto& channelInfo = _dmaDoubleBufferInfo.Channels[i];
		channelInfo.IO.CTRLA = 0;
		channelInfo.IO.CTRLB = 0;
		InterruptController::ResetService(channelInfo.InterruptCause);
	}

	TC0_t& lrClockTimer = _lrClockTimerInfo.GetTimer();
	lrClockTimer.CTRLA = 0;
	lrClockTimer.CTRLB = 0;
	lrClockTimer.CTRLC = 0;
	lrClockTimer.CTRLD = 0;

	_masterClockTimer.CTRLA = 0;
	_masterClockTimer.CTRLB = 0;
	_masterClockTimer.CTRLC = 0;
	_masterClockTimer.CTRLD = 0;

	InterruptController::ResetService(_lrClockTimerInfo.GetOverflowInterruptCause());
}

void SerialAudioSampleRenderer::Start(ISampleGenerator& generator)
{
	Stop();

	CriticalSection cs;
	{
		_sampleCount = 0;
		_peakLevelL = 0;
		_peakLevelR = 0;
		_pause = false;
		_running = true;
		_buffer.Reset(generator);		
		_buffer.Fill(generator);
		_pGenerator = &generator;
	}
}

void SerialAudioSampleRenderer::Stop(void)
{
	CriticalSection cs;
	_buffer.Reset();
	_pGenerator = nullptr;
}

void SerialAudioSampleRenderer::Pause(void)
{
	CriticalSection cs;
	_pause = !_pause;
}

bool SerialAudioSampleRenderer::IsPaused(void) const
{
	return _pause;
}

ISerialSampleRenderer::Format SerialAudioSampleRenderer::GetFormat(void) const
{
	return _format;
}

uint16_t SerialAudioSampleRenderer::GetMasterClockRatio(void) const
{
	return _masterClockRatio;
}

bool SerialAudioSampleRenderer::IsRunning(void) const
{
	CriticalSection cs;
	return _running;
}

uint32_t SerialAudioSampleRenderer::GetSampleCount(void) const
{
	CriticalSection cs;
	return _sampleCount;
}

uint32_t SerialAudioSampleRenderer::GetSamplingRate(void) const
{
	return _samplingRate;
}

void SerialAudioSampleRenderer::ResetPeakLevel(unsigned& left, unsigned& right)
{
	CriticalSection cs;
	left = _peakLevelL << 8;
	right = _peakLevelR << 8;
	_peakLevelL = 0;
	_peakLevelR = 0;
}

void SerialAudioSampleRenderer::dmaTransferCompleted(void* pContext)
{
	SerialAudioSampleRenderer& This = *static_cast<SerialAudioSampleRenderer*>(pContext);
	for (int i = 0; i < DMAChannelCount; ++i)
	{
		This.bufferSamplesForDMA(This._dmaDoubleBufferInfo.Channels[i], This._DMABuffer[i]);
	}
	This._lrClockTimerInfo.GetTimer().INTCTRLA = TC_OVFINTLVL_LO_gc;
}

void SerialAudioSampleRenderer::bufferSamplesForDMA(const DMAChannelInfo& dmaChannelInfo, Audio::SampleData samples[])
{
	auto& dmaChannel = dmaChannelInfo.IO;
	if (!(dmaChannel.CTRLB & DMA_CH_TRNIF_bm))
	{
		return;
	}

	dmaChannel.CTRLB |= DMA_CH_TRNIF_bm;

	uint8_t peakLevelL = _peakLevelL;
	uint8_t peakLevelR = _peakLevelR;

	if (_pause || !_running)
	{
		memset(samples, 0, SampleCountEachDMABuffer * sizeof(SampleData));
		peakLevelL = 0;
		peakLevelR = 0;
	}
	else
	{
		auto readedSampleCount = _buffer.ReadNext(samples, SampleCountEachDMABuffer);
		if (readedSampleCount != SampleCountEachDMABuffer)
		{
			if (readedSampleCount == 0)
			{
				if (!_buffer.IsRunning() && (_pGenerator == nullptr || !_pGenerator->IsRunning()))
				{
					_running = false;
				}
			}
			memset(&samples[readedSampleCount], 0, (SampleCountEachDMABuffer - readedSampleCount) * sizeof(SampleData));
		}
		_sampleCount += readedSampleCount;
			
		auto p = reinterpret_cast<uint8_t*>(samples);
		for (uint16_t i = 0; i < SampleCountEachDMABuffer; ++i)
		{
			uint8_t l;
			uint8_t h;
			
			//swap byte order (Left)
			l = *p;
			h = *(p + 1);
			*p = h;
			*(p + 1) = l;

			//calculate peak level (Left)
			if (h >= 0x80)
			{
				h = ~h + 1;
			}
			if (peakLevelL < h)
			{
				peakLevelL = h;
			}
			p += 2;

			//swap byte order (Right)
			l = *p;
			h = *(p + 1);
			*p = h;
			*(p + 1) = l;

			//calculate peak level (Right)
			if (h >= 0x80)
			{
				h = ~h + 1;
			}
			if (peakLevelR < h)
			{
				peakLevelR = h;
			}
			p += 2;
		}
	}

	_peakLevelL = peakLevelL;
	_peakLevelR = peakLevelR;
}

void SerialAudioSampleRenderer::timerOverflow(void* pContext)
{
	auto& This = *static_cast<SerialAudioSampleRenderer*>(pContext);
	This._lrClockTimerInfo.GetTimer().INTCTRLA = TC_OVFINTLVL_OFF_gc;
	if (This._pGenerator != nullptr)
	{
		This._buffer.Fill(*This._pGenerator);
	}		
}

void SerialAudioSampleRenderer::initDMA(const DMAChannelInfo& dmaChannelInfo, const USARTInfo& usartInfo, Audio::SampleData samples[])
{
	auto& dmaChannel = dmaChannelInfo.IO;
	memset(samples, 0, SampleCountEachDMABuffer * sizeof(SampleData));
	
	dmaChannel.SRCADDR0 = static_cast<uint8_t>((reinterpret_cast<uint16_t>(samples) >> 0) & 0xff);
	dmaChannel.SRCADDR1 = static_cast<uint8_t>((reinterpret_cast<uint16_t>(samples) >> 8) & 0xff);
#if defined(DMA_CH0_SRCADDR2)
	dmaChannel.SRCADDR2 = 0;
#endif
	dmaChannel.DESTADDR0 = static_cast<uint8_t>((reinterpret_cast<uint16_t>(&usartInfo.GetUSART()) >> 0) & 0xff);
	dmaChannel.DESTADDR1 = static_cast<uint8_t>((reinterpret_cast<uint16_t>(&usartInfo.GetUSART()) >> 8) & 0xff);
#if defined(DMA_CH0_DESTADDR2)
	dmaChannel.DESTADDR2 = 0;
#endif
	dmaChannel.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_BURST_gc | DMA_CH_DESTDIR_FIXED_gc;
	dmaChannel.TRFCNT = SampleCountEachDMABuffer * sizeof(SampleData);
	dmaChannel.TRIGSRC = usartInfo.GetDREDMATriggerSource();

	dmaChannel.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm | DMA_CH_REPEAT_bm;
	dmaChannel.CTRLB = DMA_CH_TRNINTLVL_HI_gc;
}
