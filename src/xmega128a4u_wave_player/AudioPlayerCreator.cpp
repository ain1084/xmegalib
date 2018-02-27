/*
 * AudioPlayerCreator.cpp
 *
 * Created: 2011/12/28 2:03:03
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include <Audio/WaveFile/Player.h>
#include "AudioPlayerCreator.h"

using namespace FileSystem;
using namespace Audio;

AudioPlayerCreator::AudioPlayerCreator(ISampleRenderer& renderer)
: _renderer(renderer)
{
	
}

IPlayer* AudioPlayerCreator::CreateAudioPlayer(IFile& file)
{
	return WaveFile::Player::CreateInstance(file, _renderer);
}
