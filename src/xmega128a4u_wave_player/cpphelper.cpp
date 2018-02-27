/*
 * cpphelper.cpp
 *
 * Created: 2011/08/31 1:21:19
 *  Author: Seiji Ainoguchi
 */ 

#include <stdlib.h>

void* operator new(unsigned int size)
{
	return malloc(size);
}

void* operator new[](unsigned int size)
{
	return malloc(size);
}

void operator delete(void* p)
{
	free(p);
}

void operator delete[](void* p)
{
	free(p);
}

extern "C" void __cxa_pure_virtual(void)
{
	for (;;)
		;
}
