/*
 * WaveAudioPlayer.cpp
 *
 * Created: 2011/12/30 4:11:46
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "FileSystem/IFile.h"
#include "Audio/ISampleRenderer.h"
#include "HeaderInfo.h"
#include "HeaderInfoParser.h"
#include "Player.h"

using namespace FileSystem;
using namespace Audio;
using namespace Audio::WaveFile;

IPlayer* Player::CreateInstance(IFile& file, ISampleRenderer& renderer)
{
	HeaderInfo wfi;
	Metadata metadata;
	bool supported = HeaderInfoParser::Parse(file, wfi, metadata);
	file.Seek(IFile::Seek_Top, 0);
	if (!supported)
	{
		return nullptr;
	}		
	if (wfi.formatTag != 1)
	{
		return nullptr;
	}
	if (wfi.channels > 2)
	{
		return nullptr;
	}
	if (wfi.bitsPerSamples != 16)
	{
		return nullptr;
	}
	if (renderer.GetSamplingRate() != wfi.samplesPerSec)
	{
		return nullptr;
	}
	return new Player(file, wfi, metadata, renderer);
}



Player::Player(IFile& file, const HeaderInfo& headerInfo, const Metadata& metadata, ISampleRenderer& renderer)
 : _renderer(renderer)
 , _generator(file, headerInfo)
 , _metadata(metadata)
{

}

Player::~Player(void)
{
	End();
}

void Player::GetMetadata(Metadata& metadata) const
{
	metadata = _metadata;
}

bool Player::Play(void)
{
	End();
	_renderer.Start(_generator);
	return true;	
}

void Player::End(void)
{
	_renderer.Stop();
}

bool Player::IsPlaying(void) const
{
	return _renderer.IsRunning();
}

void Player::Pause(void)
{
	return _renderer.Pause();	
}

bool Player::IsPaused(void) const
{
	return _renderer.IsPaused();
}	

uint32_t Player::GetSampleCount(void) const
{
	return _renderer.GetSampleCount();
}

uint32_t Player::GetTotalSampleCount(void) const
{
	return _generator.GetTotalSampleCount();
}
