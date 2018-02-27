/*
 * Player.h
 *
 * Created: 2011/12/30 4:11:56
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Audio/IPlayer.h"
#include "Audio/Metadata.h"
#include "SampleGenerator.h"

namespace FileSystem
{
	class IFile;
}

namespace Audio
{
	class ISampleRenderer;
	struct HeaderInfo;
	
	namespace WaveFile
	{
		class Player : public IPlayer
		{
		public:
			static IPlayer* CreateInstance(FileSystem::IFile& file, ISampleRenderer& renderer);
			virtual ~Player(void);

			virtual void GetMetadata(Metadata& metadata) const;
			virtual bool Play(void);
			virtual void End(void);
			virtual bool IsPlaying(void) const;
			virtual void Pause(void);
			virtual bool IsPaused(void) const;
			virtual uint32_t GetSampleCount(void) const;
			virtual uint32_t GetTotalSampleCount(void) const;

		private:
			Player(FileSystem::IFile& file, const HeaderInfo& wfi, const Metadata& metadata, ISampleRenderer& renderer);

			ISampleRenderer& _renderer;
			SampleGenerator _generator;
			const Metadata _metadata;
		};
	}
}
