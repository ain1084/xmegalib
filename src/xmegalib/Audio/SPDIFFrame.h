/*
 * SPDIFFrame.h
 *
 * Created: 2020/12/15 1:58:28
 *  Author: ain
 */ 

#pragma once

#include "SPDIFSubFrame.h"

namespace Audio
{
	#pragma pack(push ,1)
	
	struct SPDIFFrame
	{
		SPDIFSubFrame L;
		SPDIFSubFrame R;
	};
	
	#pragma pack(pop)
}
