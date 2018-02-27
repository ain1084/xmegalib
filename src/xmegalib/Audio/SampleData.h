/*
 * SampleData.h
 *
 * Created: 2012/02/11 0:09:39
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

namespace Audio
{
	#pragma pack(push ,1)
	struct SampleData
	{
		int16_t L;
		int16_t R;
	};
	#pragma pack(pop)
}
