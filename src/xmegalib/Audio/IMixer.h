/*
 * IMixer.h
 *
 * Created: 2011/12/05 1:30:05
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace Audio
{
	class IMixer
	{
	public:
		enum
		{
			VOLUME_DEFAULT_PERCENT = 50,
			VOLUME_MIN_PERCENT = 0,
			VOLUME_MAX_PERCENT = 100
		};
	
		virtual int GetVolume(void) const = 0;
		virtual int SetVolume(int percent) = 0;
		virtual ~IMixer(void) { }
	};
}
