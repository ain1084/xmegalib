/*
 * StringUtilities.cpp
 *
 * Created: 2012/06/18 2:16:37
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include "StringUtilities.h"

char* StringUtilities::StripRightSpace(char* pStr)
{
	char* p = pStr + strlen(pStr);
	if (p != pStr)
	{
		do 
		{
			if (*(p - 1) != ' ')
			{
				*p = '\0';
				break;
			}
		} while (--p != pStr);
	}
	return pStr;
}