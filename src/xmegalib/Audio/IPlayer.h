/*
 * IPlayer.h
 *
 * Created: 2011/12/15 23:32:38
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace Audio
{
	struct Metadata;

	class IPlayer
	{
	public:	
		virtual ~IPlayer(void) { }

		virtual void GetMetadata(Metadata& metadata) const = 0;
		virtual bool Play(void) = 0;
		virtual void End(void) = 0;
		virtual bool IsPlaying(void) const = 0;
		virtual void Pause(void) = 0;
		virtual bool IsPaused(void) const = 0;
		virtual uint32_t GetSampleCount(void) const = 0;
		virtual uint32_t GetTotalSampleCount(void) const = 0;
	};
}
