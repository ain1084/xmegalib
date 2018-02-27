/*
 * Metadata.h
 *
 * Created: 2012/05/15 1:29:19
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Utilities/BasicString.h"

namespace Audio
{
	struct Metadata
	{
		BasicString ArtistName;
		BasicString Name;
		BasicString ProductName;
		BasicString Genre;
	};
}
