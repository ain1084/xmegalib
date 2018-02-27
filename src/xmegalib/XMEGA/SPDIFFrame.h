/*
 * SPDIFFrame.h
 *
 * Created: 2016/02/10 0:10:45
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

namespace XMEGA
{
	#pragma pack(push ,1)
	struct SPDIFSubFrame
	{
		uint8_t Preamble;		// 00-03
		uint8_t Aux;			// 04-07
		uint8_t Audio[5];		// 08-27
		union
		{
			struct
			{
				uint8_t Validity:2;		// 28
				uint8_t User:2;			// 29
				uint8_t Status:2;		// 30
				uint8_t Parity:2;		// 31
			};
			uint8_t Data;
		};
	};
	
	struct SPDIFFrame
	{
		SPDIFSubFrame L;
		SPDIFSubFrame R;	
	};
	
	#pragma pack(pop)
}
