/*
 * SerialAudioSampleRenderer.h
 *
 * Created: 2011/11/28 01:09:00
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Audio/ISampleRenderer.h"
#include "Audio/SampleData.h"
#include "Audio/SampleBuffer.h"
#include "TimerCounter0Info.h"
#include "Audio/SPDIFFrame.h"
#include "SPDIFFrameEncoder.h"

namespace XMEGA
{
	class USARTInfo;
	enum class DMADoubleBufferMode;
	class DMADoubleBufferInfo;

	class SPDIFAudioSampleRenderer : public Audio::ISampleRenderer
	{
	public:
	
		SPDIFAudioSampleRenderer(USART_t& dataPort,
								 DMADoubleBufferMode dmaDoubleBufferMode,
								 TC0_t& lrClockTimer,
								 uint32_t samplingRate);
	
		virtual ~SPDIFAudioSampleRenderer(void);

		virtual void Start(Audio::ISampleGenerator& generator);
		virtual void Stop(void);
		virtual void Pause(void);
		virtual bool IsPaused(void) const;
		virtual bool IsRunning(void) const;
		virtual uint32_t GetSampleCount(void) const;
		virtual uint32_t GetSamplingRate(void) const;
		virtual void ResetPeakLevel(unsigned& left, unsigned& right);
	
	private:

		enum
		{
			SampleCountEachDMABuffer = 64,
			DMAChannelCount = 2
		};

		volatile USART_t& _dataUSART;
		const DMADoubleBufferInfo& _dmaDoubleBufferInfo;
		TC0_t& _lrClockTimer;
		const uint32_t _samplingRate;
		Audio::SampleBuffer _buffer;
		Audio::ISampleGenerator* _pGenerator;
		Audio::SPDIFFrame _DMABuffer[DMAChannelCount][SampleCountEachDMABuffer];
		SPDIFFrameEncoder _spdifEncoder;
		uint32_t _sampleCount;
		uint8_t _peakLevelL;
		uint8_t _peakLevelR;
		bool _pause;
		bool _running;

		static void dmaTransferCompleted(void* pContext);
		static void timerOverflow(void* pContext);
		static void setupDMA(volatile DMA_CH_t& dmaChannel, Audio::SPDIFFrame spdifFrame[]);
		void bufferSamplesForDMA(volatile DMA_CH_t& dmaCh, uint8_t bufferNumber);
		void initBuffer(uint_fast8_t channelNumber);
		void startDMA(void);
		void stopDMA(void);
	};
}
