/*
 * MixerCollection.cpp
 *
 * Created: 2012/09/30 22:11:13
 *  Author: Seiji Ainoguchi
 */ 

#include "IMixer.h"
#include "MixerCollection.h"

using namespace Audio;

MixerCollection::MixerCollection(void)
 : _volume(IMixer::VOLUME_DEFAULT_PERCENT)
{
	updateVolume();
}



void MixerCollection::Add(IMixer& mixer)
{
	_container.Add(&mixer);
}

bool MixerCollection::Remove(IMixer& mixer)
{
	return _container.Remove(&mixer);	
}

int MixerCollection::GetVolume(void) const
{
	return _volume;
}

void MixerCollection::SetVolume(int volume)
{
	if (volume < IMixer::VOLUME_MIN_PERCENT)
	{
		volume = IMixer::VOLUME_MIN_PERCENT;
	}
	else if (volume >= IMixer::VOLUME_MAX_PERCENT)
	{
		volume = IMixer::VOLUME_MAX_PERCENT;
	}
	
	_volume = volume;
	updateVolume();
}

void MixerCollection::updateVolume(void)
{
	for (Container::IndexType i = 0; i < _container.GetCount(); ++i)
	{
		_container[i]->SetVolume(_volume);
	}
}