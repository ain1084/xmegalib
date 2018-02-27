/*
 * MultiByteString.h
 *
 * Created: 2012/05/07 22:53:22
 *  Author: Seiji Ainoguchi
 */ 

#include <stdint.h>

class BasicString;
class MultiByteString
{
public:
	static bool IsLeadByte(char ch)
	{
		return (ch >= 0x81 && ch <= 0x9f) || (ch >= 0xe0 && ch <= 0xee);
	}

	static bool Is2ndByte(char ch)
	{
		return (ch >= 0x40 && ch <= 0x7e) || (ch >= 0x80 && ch <= 0xfc);
	}
	
	static uint16_t ZenkakuToHankaku(uint16_t wchar);
	
	static BasicString ZenkakuToHankaku(const char* pStr);
};