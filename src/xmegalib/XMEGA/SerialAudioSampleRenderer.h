/*
 * SerialAudioSampleRenderer.h
 *
 * Created: 2011/11/28 01:09:00
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Audio/ISerialSampleRenderer.h"

#include "Audio/SampleData.h"
#include "Audio/SampleBuffer.h"
#include "TimerCounter0Info.h"



namespace XMEGA
{
	class USARTInfo;
	enum class DMADoubleBufferMode;
	struct DMADoubleBufferInfo;
	struct DMAChannelInfo;

	class SerialAudioSampleRenderer : public Audio::ISerialSampleRenderer
	{
	public:
	
		SerialAudioSampleRenderer(USART_t& dataPort,
								  DMADoubleBufferMode dmaDoubleBufferMode,
								  TC0_t& lrClockTimer, TC1_t& masterClockTimer,
								  uint32_t samplingRate, uint16_t masterClockRatio,
								  Format format = Format_I2S);
	
		virtual ~SerialAudioSampleRenderer(void);

		virtual void Start(Audio::ISampleGenerator& generator);
		virtual void Stop(void);
		virtual void Pause(void);
		virtual bool IsPaused(void) const;
		virtual bool IsRunning(void) const;
		virtual uint32_t GetSampleCount(void) const;
		virtual uint32_t GetSamplingRate(void) const;
		virtual Format GetFormat(void) const;
		virtual uint16_t GetMasterClockRatio(void) const;
		virtual void ResetPeakLevel(unsigned& left, unsigned& right);
	
	private:

		enum
		{
			SampleCountEachDMABuffer = 32,
			DMAChannelCount = 2
		};
		volatile USART_t& _dataUSART;
		const DMADoubleBufferInfo& _dmaDoubleBufferInfo;
		TimerCounter0Info _lrClockTimerInfo;
		volatile TC1_t& _masterClockTimer;
		const uint32_t _samplingRate;
		Audio::SampleBuffer _buffer;
		Audio::ISampleGenerator* _pGenerator;
		Audio::SampleData _DMABuffer[DMAChannelCount][SampleCountEachDMABuffer];
		Format _format;
		uint16_t _masterClockRatio;
		uint32_t _sampleCount;
		uint8_t _peakLevelL;
		uint8_t _peakLevelR;
		bool _pause;
		bool _running;

		static void dmaTransferCompleted(void* pContext);
		static void timerOverflow(void* pContext);
		void bufferSamplesForDMA(const DMAChannelInfo& dmaChannelInfo, Audio::SampleData sampleBuffer[]);
		void initBuffer(uint_fast8_t channelNumber);
		static void initDMA(const DMAChannelInfo& dmaChannelInfo, const USARTInfo& usartInfo, Audio::SampleData samples[]);
	};
}
