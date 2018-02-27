/*
 * FontDataManager.cpp
 *
 * Created: 2012/06/05 0:30:24
 *  Author: Seiji Ainoguchi
 */ 

#include "Font.h"
#include "FontDataManager.h"

using namespace Graphics;

FontDataManager FontDataManager::_instance;

void FontDataManager::Add(IFontData& fontData)
{
	_container.Add(&fontData);
}

bool FontDataManager::Remove(IFontData& fontData)
{
	return _container.Remove(&fontData);
}

Font* FontDataManager::CreateFont(uint_fast8_t width, uint_fast8_t height)
{
	IFontData* pHankaku;
	IFontData* pZenkaku;
	getFontPair(width, height, &pHankaku, &pZenkaku);
	if (pHankaku != nullptr || pZenkaku != nullptr)
	{
		return new Font(pHankaku, pZenkaku);
	}
	else
	{
		return nullptr;
	}
}

void FontDataManager::getFontPair(int width, int height, IFontData** ppHankaku, IFontData** ppZenkaku)
{
	*ppHankaku = nullptr;
	*ppZenkaku = nullptr;

	for (Container::IndexType i = 0; i < _container.GetCount(); ++i)
	{
		IFontData* pFontData = _container[i];
		if (width == 0)
		{
			if (pFontData->GetCodeType() == IFontData::Hankaku && *ppHankaku == nullptr)
			{
				if (*ppZenkaku != nullptr)
				{
					if (height == pFontData->GetHeight())
					{
						if (width == pFontData->GetWidth())
						{
							*ppHankaku = pFontData;
						}
					}
				}
				else
				{
					*ppHankaku = pFontData;
					width = pFontData->GetWidth();
					height = pFontData->GetHeight();
				}					
			}
			else
			{
				if (*ppZenkaku == nullptr)
				{
					if (*ppHankaku != nullptr)
					{
						if (height == pFontData->GetHeight())
						{
							if (width < pFontData->GetWidth())
							{
								*ppZenkaku = pFontData;
							}
						}
					}
					else
					{
						*ppZenkaku = pFontData;
						width = pFontData->GetWidth() >> 1;
						height = pFontData->GetHeight();
					}
				}
			}
		}
		else
		{
			if (height == pFontData->GetHeight())
			{
				if (pFontData->GetCodeType() == IFontData::Hankaku)
				{
					if (width == pFontData->GetWidth())
					{
						*ppHankaku = pFontData;
					}
				}
				else
				{
					if (width < pFontData->GetWidth())
					{
						*ppZenkaku = pFontData;
					}
				}
			}			
		}
	}			
}
