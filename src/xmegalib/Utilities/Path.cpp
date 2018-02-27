/*
 * Path.cpp
 *
 * Created: 2012/05/07 16:19:17
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include "BasicString.h"
#include "Collections/Vector.h"
#include "Utilities/MultiByteString.h"
#include "Path.h"

using namespace Collections;

BasicString Path::Combine(const char* pPath1, const char* pPath2)
{
	int path1Length = strlen(pPath1);
	int path2Length = strlen(pPath2);
	Vector<char> buf(path1Length + path2Length + 1 + 1);
	char ch;
	char *p = &buf[0];
	unsigned lastChar = 0;
	while ((ch = *pPath1++) != '\0')
	{
		*p++ = ch;
		lastChar = ch;
		if (MultiByteString::IsLeadByte(ch))
		{
			char secondChar = *pPath1;
			lastChar = ch << 8 | secondChar;
			if (MultiByteString::Is2ndByte(secondChar))
			{
				*p++ = secondChar;
				++pPath1;
			}
		}
	}
	if (lastChar != '\\' && lastChar != '/')
	{
		*p++ = '/';
	}
	strcpy(p, pPath2);
	return &buf[0];
}

BasicString Path::GetName(const char* pFullName)
{
	char ch;
	const char *pLastDelimiter = nullptr;
	const char *p = pFullName;
	
	while ((ch = *p++) != '\0')
	{
		if (!MultiByteString::IsLeadByte(ch))
		{
			if (ch == '\\' || ch == '/')
			{
				pLastDelimiter = p;
			}
		}
		else
		{
			if (MultiByteString::Is2ndByte(*p))
			{
				++p;
			}
		}
	}
	if (pLastDelimiter == nullptr)
	{
		pLastDelimiter = pFullName;
	}
	return pLastDelimiter;
}
