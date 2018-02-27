/*
 * FontDataManager.h
 *
 * Created: 2012/06/05 0:30:11
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Collections/Vector.h"
#include "IFontData.h"

namespace Graphics
{
	class Font;

	class FontDataManager
	{
	public:
		static FontDataManager& GetInstance(void)
		{
			return _instance;
		}
	
		void Add(IFontData& fontData);
		bool Remove(IFontData& fontData);
		
		Font* CreateFont(uint_fast8_t width, uint_fast8_t height);
	
	private:
		static FontDataManager _instance;

		typedef Collections::Vector<IFontData*> Container;
		Container _container;

		void getFontPair(int width, int height, IFontData** ppZenkaku, IFontData** ppHankaku);
	};
}	