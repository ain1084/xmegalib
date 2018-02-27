/*
 * MixerCollection.h
 *
 * Created: 2012/09/30 22:11:02
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

//#include "Platform.h"
#include <stdint.h>
#include "Collections/Vector.h"

namespace Audio
{
	class IMixer;

	class MixerCollection
	{
	public:
		MixerCollection(void);
	
		void Add(IMixer& mixer);
		bool Remove(IMixer& mixer);

		int GetVolume(void) const;
		void SetVolume(int volume);

	private:
		void updateVolume(void);

		int _volume;
		typedef Collections::Vector<IMixer*, uint8_t> Container;
		Container _container;
	};
}
