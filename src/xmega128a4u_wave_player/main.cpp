/*
 * xmega128a4u_audio_test.cpp
 *
 * Created: 2016/01/31 18:13:18
 * Author : Seiji Ainoguchi
 */ 

#include <XMEGA/Platform.h>
#include <XMEGA/InterruptController.h>
#include <XMEGA/SystemClock.h>
#include <XMEGA/SerialAudioSampleRenderer.h>
#include <XMEGA/SPDIFAudioSampleRenderer.h>
#include <XMEGA/DMAInfo.h>
#include <Device/CS2200.h>
#include <Device/SRAM23K.h>
#include <Device/AT45DB.h>
#include <Device/LEDPin.h>
#include <Device/MAX9850AudioMixer.h>
#include <Device/LM49450AudioMixer.h>
#include <FileSystem/IFile.h>
#include <FileSystem/IFileInfo.h>
#include <FileSystem/FAT/MMCDiskIO.h>
#include <FileSystem/FAT/DiskIOManager.h>
#include <FileSystem/FAT/DirectoryInfo.h>
#include <FileSystem/Enumerator.h>
#include <Audio/ISampleRenderer.h>
#include <Audio/IPlayer.h>
#include <Device/PushButton.h>
#include <Device/RepeatButton.h>
#include <Utilities/ScopedPtr.h>
#include "AudioPlayerCreator.h"

using namespace FileSystem;

void start(AudioPlayerCreator& playerCreator, Device::PushButton& nextButton)
{
	// key scan timer
	const int KEY_SCAN_INTERVAL_MSEC = 10;
	SystemClockTimer keyInputTimer(SystemClockCounter::MStoClock(KEY_SCAN_INTERVAL_MSEC));
	
	const int REPEAT_START_TIME = 500 / KEY_SCAN_INTERVAL_MSEC;		// 500ms / scan interval 
	const int REPEAT_INTERVAL_TIME = 1;	
	Device::RepeatButton repeatButton;
	repeatButton.Add(nextButton);
	repeatButton.SetTime(REPEAT_START_TIME, REPEAT_INTERVAL_TIME);

	for (;;)
	{
		Enumerator ed(FAT::DirectoryInfo::Create(""), 3);
		uint16_t total = ed.FetchCount();
		int index = 0;
		int scanDir = 1;
		for (;;)
		{
			if (index < 0)
			{
				index = total - 1;
			}
			else if (static_cast<unsigned>(index) >= total)
			{
				index = 0;
			}
			
			ScopedPtr<IFileInfo> pFileInfo(ed.FetchIndexOf(index));
			if (!pFileInfo)
			{
				index += scanDir;
				continue;
			}
			ScopedPtr<IFile> pFile(pFileInfo->OpenRead());
			if (!pFile)
			{
				index += scanDir;
				continue;
			}
			ScopedPtr<Audio::IPlayer> pPlayer(playerCreator.CreateAudioPlayer(*pFile.Get()));
			if (!pPlayer)
			{
				index += scanDir;
				continue;
			}

			pPlayer->Play();

			for (;;)
			{
				if (!pPlayer->IsPlaying())
				{
					scanDir = 1;
					index++;
					break;
				}
				
				if (!keyInputTimer.IsReached())
				{
					continue;
				}
				keyInputTimer.Reset();

				repeatButton.Update();
				Device::PushButton* pRepeatButton = repeatButton.GetPushed();

				if (pRepeatButton == &nextButton)
				{
					scanDir = 1;
					index += 1;
					break;
				}
			}
			if (pPlayer)
			{
				pPlayer->End();
			}
		}
	}
}

#include <XMEGA/GPIOI2CMaster.h>

int main(void)
 {
	XMEGA::InterruptController::Start();
	XMEGA::SystemClockCounter::Start();

	const uint32_t HZ_TO_MHZ = 1000 * 1000UL;	 
	const uint32_t REF_CLOCK = 10 * HZ_TO_MHZ;
	const uint32_t samplingRate = 44100;
	const uint16_t mclkRatio = 256;

	I2CMaster i2cMaster(TWIE, 10000);
		
	//Set external clock
	const uint8_t CS2200_I2C_ADDRESS = Device::CS2200::DefaultAD0;
	Device::CS2200 cs2200(i2cMaster, CS2200_I2C_ADDRESS, REF_CLOCK);
	cs2200.SetOutputClock(static_cast<uint32_t>(samplingRate) * mclkRatio);
	SystemClockTimer::WaitForReached(SystemClockCounter::MStoClock(1));
	
	//Set clock system
	auto& systemClock = SystemClock::GetInstance();
	systemClock.SetExternalOscillatorFrequency(static_cast<uint32_t>(samplingRate) * mclkRatio);
	systemClock.EnableOscillator(SystemClock::Oscillator_External);
	systemClock.SetPrescaler(CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc);
	systemClock.SetPLL(SystemClock::PLLSource_External, 4);
	systemClock.EnableOscillator(SystemClock::Oscillator_PLL);
	systemClock.SetClockSource(SystemClock::Oscillator_PLL);
	systemClock.NotifyClockUpdated();

	SPIMaster spi(USARTC1);
	
	//SRAM
	const uint32_t  SRAM_MAX_FREQUENCY = 20 * HZ_TO_MHZ; 
	OutputPin sramCSPin(PORTE, PIN3_bp);
	SPIChip sramChip(spi, sramCSPin, SRAM_MAX_FREQUENCY);
	Device::SRAM23K sram23K(sramChip);

	//DataFlash
	const uint32_t AT45_MAX_FREQUENCY = 66 * HZ_TO_MHZ;
	OutputPin dataFlashCSPin(PORTE, PIN2_bp);
	SPIChip dataFlashChip(spi, dataFlashCSPin, AT45_MAX_FREQUENCY);
	Device::IEEPROM* pEEPROM = Device::AT45DB::Create(dataFlashChip);
	
	//MicroSD card
	OutputPin sdAccessLEDPin(PORTD, PIN2_bp);
	Device::LEDPin sdAccessLED(sdAccessLEDPin);
	OutputPin microSDCSPin(PORTC, PIN4_bp);
	InputPin sdcardDetect(PORTC, PIN0_bp);
	const uint32_t SDCARD_MAX_FREQUENCY = 25 * HZ_TO_MHZ;
	SPIChip microsdChip(spi, microSDCSPin, sdAccessLED, SDCARD_MAX_FREQUENCY);
	
	//File system
	FileSystem::FAT::MMCDiskIO diskIO(microsdChip, sdcardDetect);
	FileSystem::FAT::DiskIOManager::Add(diskIO);

	XMEGA::InputPin userButtonPin(PORTD, PIN5_bp);
	Device::PushButton userButton(userButtonPin);
	
	XMEGA::SerialAudioSampleRenderer renderer(USARTD0, DMADoubleBufferMode::Channel_2_3, TCD0, TCD1, samplingRate, 256, Audio::ISerialSampleRenderer::Format::Format_I2S);//::Format_LeftJustified);
//	XMEGA::SPDIFAudioSampleRenderer renderer(USARTD0, DMADoubleBufferMode::Channel_2_3, TCD0, samplingRate);

 	Device::MAX9850AudioMixer mixier1(renderer, i2cMaster, 0x10);
	mixier1.SetVolume(60);

	Device::LM49450AudioMixer mixier2(renderer, i2cMaster, 0x7d);
	mixier2.SetVolume(50);

	AudioPlayerCreator playerCreator(renderer);
		
	start(playerCreator, userButton);

	return 0;
}



