/*
 * Path.h
 *
 * Created: 2012/05/07 16:18:19
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "BasicString.h"

class Path
{
public:
	static BasicString Combine(const char* pPath1, const char* pPath2);
	static BasicString GetName(const char* pFullName);
};
