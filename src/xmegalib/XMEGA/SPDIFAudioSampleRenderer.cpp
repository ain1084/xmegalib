/*
 * PCMDACAudioSampleRenderer.cpp
 *
 * Created: 2011/09/05 23:49:36
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include <limits.h>
#include "Platform.h"
#include "Audio/ISampleGenerator.h"
#include "SystemClock.h"
#include "InterruptController.h"
#include "USARTInfo.h"
#include "PortInfo.h"
#include "TimerCounter1Info.h"
#include "DMAInfo.h"
#include "SPDIFAudioSampleRenderer.h"

using namespace XMEGA;
using namespace Audio;

SPDIFAudioSampleRenderer::SPDIFAudioSampleRenderer(USART_t& dataUSART,
 DMADoubleBufferMode dmaDoubleBufferMode,
 TC0_t& lrClockTimer, uint32_t samplingRate)
 : _dataUSART(dataUSART)
 , _dmaDoubleBufferInfo(DMAInfo::GetDoubleBufferInfo(dmaDoubleBufferMode))
 , _lrClockTimer(lrClockTimer)
 , _samplingRate(samplingRate)
 , _buffer(256)
 , _pGenerator(0)
 , _sampleCount(0)
 , _peakLevelL(0)
 , _peakLevelR(0)
 , _pause(false)
 , _running(false)
{	
	CriticalSection cs;
	
	//Initialize USART for S/PDIF output
	auto clockPerFrame = sizeof(SPDIFFrame) * CHAR_BIT;
	auto sclkRate  = samplingRate * clockPerFrame;
	auto peripheralClock = SystemClock::GetInstance().GetPeripheralClock();
	auto usartBaud = static_cast<uint16_t>((peripheralClock / sclkRate) / 2 - 1);	
	dataUSART.CTRLA = 0;
	dataUSART.CTRLB = USART_TXEN_bm;
	dataUSART.CTRLC = USART_CMODE_MSPI_gc;
	dataUSART.BAUDCTRLA = static_cast<uint8_t>((usartBaud & 0x00ff) >> 0);
	dataUSART.BAUDCTRLB = static_cast<uint8_t>((usartBaud & 0xff00) >> 8);
	USARTInfo dataUSARTInfo(dataUSART);
	auto& dataPort = dataUSARTInfo.GetPort();
	dataPort.DIRSET = dataUSARTInfo.GetTXDBitMask();

	//Initialize timer for LRCLK
	lrClockTimer.CTRLA = TC_CLKSEL_DIV1_gc;
	lrClockTimer.CTRLB = TC_WGMODE_FRQ_gc;
	lrClockTimer.CTRLC = 0;
	lrClockTimer.CTRLD = 0;
	lrClockTimer.CNT = 0;
	lrClockTimer.CCA = clockPerFrame * SampleCountEachDMABuffer * (usartBaud + 1);
	lrClockTimer.INTCTRLA = TC_OVFINTLVL_LO_gc;
	
	//initialize interrupt DMA, timer
	for (auto i = 0; i < 2; ++i)
	{
		auto& dmaChannelInfo = _dmaDoubleBufferInfo.Channels[i];
		InterruptController::SetService(dmaChannelInfo.InterruptCause, dmaTransferCompleted, this);
	}
	InterruptController::SetService(TimerCounter0Info(lrClockTimer).GetOverflowInterruptCause(), timerOverflow, this);
}

SPDIFAudioSampleRenderer::~SPDIFAudioSampleRenderer(void)
{
	CriticalSection cs;

	Stop();
	for (auto i = 0; i < 2; ++i)
	{
		auto& dmaChannelInfo = _dmaDoubleBufferInfo.Channels[i];
		dmaChannelInfo.IO.CTRLA = 0;
		dmaChannelInfo.IO.CTRLB = 0;
		InterruptController::ResetService(dmaChannelInfo.InterruptCause);
	}

	TimerCounter0Info lrClockTimerInfo(_lrClockTimer);
	TC0_t& lrClockTimer =  lrClockTimerInfo.GetTimer();
	lrClockTimer.CTRLA = 0;
	lrClockTimer.CTRLB = 0;
	lrClockTimer.CTRLC = 0;
	lrClockTimer.CTRLD = 0;
	InterruptController::ResetService(lrClockTimerInfo.GetOverflowInterruptCause());
}

void SPDIFAudioSampleRenderer::Start(ISampleGenerator& generator)
{
	CriticalSection cs;

	Stop();

	_sampleCount = 0;
	_peakLevelL = 0;
	_peakLevelR = 0;
	_pause = false;
	_running = true;
	_buffer.Reset(generator);		
	_buffer.Fill(generator);
	_pGenerator = &generator;

	_lrClockTimer.CTRLB |= TC1_CCAEN_bm;
	startDMA();
}

void SPDIFAudioSampleRenderer::Stop(void)
{
	CriticalSection cs;

	stopDMA();
	_lrClockTimer.CTRLB &= ~TC1_CCAEN_bm;
	_buffer.Reset();
	_pGenerator = nullptr;
}

void SPDIFAudioSampleRenderer::Pause(void)
{
	CriticalSection cs;

	_pause = !_pause;
}

bool SPDIFAudioSampleRenderer::IsPaused(void) const
{
	return _pause;
}

bool SPDIFAudioSampleRenderer::IsRunning(void) const
{
	CriticalSection cs;

	return _running;
}

uint32_t SPDIFAudioSampleRenderer::GetSampleCount(void) const
{
	CriticalSection cs;

	return _sampleCount;
}

uint32_t SPDIFAudioSampleRenderer::GetSamplingRate(void) const
{
	return _samplingRate;
}

void SPDIFAudioSampleRenderer::ResetPeakLevel(unsigned& left, unsigned& right)
{
	CriticalSection cs;

	left = _peakLevelL << 8;
	right = _peakLevelR << 8;
	_peakLevelL = 0;
	_peakLevelR = 0;
}

void SPDIFAudioSampleRenderer::dmaTransferCompleted(void* pContext)
{
	auto& This = *static_cast<SPDIFAudioSampleRenderer*>(pContext);
	for (auto i = 0; i < 2; ++i)
	{
		auto& dmaChannelInfo = This._dmaDoubleBufferInfo.Channels[i];
		This.bufferSamplesForDMA(dmaChannelInfo.IO, i);
	}
}

void SPDIFAudioSampleRenderer::bufferSamplesForDMA(volatile DMA_CH_t& dmaCh, uint8_t bufferNumber)
{
	if (!(dmaCh.CTRLB & DMA_CH_TRNIF_bm))
	{
		return;
	}

	dmaCh.CTRLB |= DMA_CH_TRNIF_bm;

	auto peakLevelL = _peakLevelL;
	auto peakLevelR = _peakLevelR;
	
	Audio::SampleData samples[SampleCountEachDMABuffer];

	if (_pause || !_running)
	{
		memset(&samples, 0, sizeof(samples));
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
				if (_pGenerator == nullptr || !_pGenerator->IsRunning())
				{
					_running = false;
				}
			}
			memset(&samples[readedSampleCount], 0, (SampleCountEachDMABuffer - readedSampleCount) * sizeof(SampleData));
		}
		_sampleCount += readedSampleCount;
	}

	_spdifEncoder.Encode(samples, _DMABuffer[bufferNumber], SampleCountEachDMABuffer);
}

void SPDIFAudioSampleRenderer::timerOverflow(void* pContext)
{
	auto& This = *static_cast<SPDIFAudioSampleRenderer*>(pContext);
	if (This._pGenerator != nullptr)
	{
		This._buffer.Fill(*This._pGenerator);
	}		
}

void SPDIFAudioSampleRenderer::startDMA(void)
{
	USARTInfo dataUSARTInfo(_dataUSART);

	for (auto i = 0; i < 2; ++i)
	{
		auto& dmaChannel = _dmaDoubleBufferInfo.Channels[i].IO;
		memset(_DMABuffer[i], 0, sizeof(SPDIFFrame) * SampleCountEachDMABuffer);
	
		dmaChannel.SRCADDR0 = static_cast<uint8_t>((reinterpret_cast<uint16_t>(_DMABuffer[i]) >> 0) & 0xff);
		dmaChannel.SRCADDR1 = static_cast<uint8_t>((reinterpret_cast<uint16_t>(_DMABuffer[i]) >> 8) & 0xff);
		#if defined(DMA_CH0_SRCADDR2)
		dmaChannel.SRCADDR2 = 0;
		#endif
		dmaChannel.DESTADDR0 = static_cast<uint8_t>((reinterpret_cast<uint16_t>(&dataUSARTInfo.GetUSART()) >> 0) & 0xff);
		dmaChannel.DESTADDR1 = static_cast<uint8_t>((reinterpret_cast<uint16_t>(&dataUSARTInfo.GetUSART()) >> 8) & 0xff);
		#if defined(DMA_CH0_DESTADDR2)
		dmaChannel.DESTADDR2 = 0;
		#endif
		dmaChannel.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_BURST_gc | DMA_CH_DESTDIR_FIXED_gc;
		dmaChannel.TRFCNT = sizeof(SPDIFFrame) * SampleCountEachDMABuffer;
		dmaChannel.REPCNT = 0;
		dmaChannel.TRIGSRC = dataUSARTInfo.GetDREDMATriggerSource();

		dmaChannel.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm | DMA_CH_REPEAT_bm;
		dmaChannel.CTRLB = DMA_CH_TRNINTLVL_HI_gc;
	}

	DMA_CTRL |= DMA_ENABLE_bm | _dmaDoubleBufferInfo.dbufMode;
	auto& firstDmaChannel = _dmaDoubleBufferInfo.Channels[0].IO;
	#if defined(DMA_CH_CHEN_bm)
	firstDmaChannel.CTRLA |= DMA_CH_CHEN_bm;
	#else
	firstDmaChannel.CTRLA |= DMA_CH_ENABLE_bm;
	#endif
}

void SPDIFAudioSampleRenderer::stopDMA(void)
{
	DMA_CTRL &=  ~_dmaDoubleBufferInfo.dbufMode | ~DMA_ENABLE_bm;
	for (auto i = 0; i < 2; ++i)
	{
		auto& dmaChannelIO = _dmaDoubleBufferInfo.Channels[i].IO;
		#if defined(DMA_CH_CHEN_bm)
		dmaChannelIO.CTRLA &= ~DMA_CH_CHEN_bm;
		#else
		dmaChannelIO.CTRLA &= ~DMA_CH_ENABLE_bm;
		#endif
	}
}
