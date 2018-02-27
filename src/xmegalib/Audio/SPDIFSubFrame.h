/*
 * SPDIFSubFrame.h
 *
 * Created: 2016/02/06 16:18:27
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace Audio
{
	#pragma pack(push ,1)
	struct SPDIFSubFrame
	{
		uint8_t Preamble;		// 00-03
		uint8_t AuxData;		// 04-07
		uint8_t AudioData[5];	// 08-27
		uint8_t Validity:2;		// 28
		uint8_t User:2;			// 29
		uint8_t Status:2;		// 30
		uint8_t Parity:2;		// 31
	};
	#pragma pack(pop)
}
