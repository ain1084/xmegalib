/*
 * BasicString.h
 *
 * Created: 2012/05/04 23:31:25
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <string.h>

class BasicString
{
private:
	class Data
	{
	public:
		Data(const char* pString)
		 : _refCount(1)
		{
			if (pString == nullptr)
			{
				pString = "";
			}
			int length = strlen(pString) + 1;
			_pStr = new char[length];
			memcpy(_pStr, pString, length);
		}
		
		Data(const char* pString, int count)
		 : _refCount(1)
		{
			_pStr = new char[count + 1];
			memcpy(_pStr, pString, count);
			*(_pStr + count) = '\0';
		}
		
		~Data(void)
		{
			delete[] _pStr;
		}
		
		void AddRef(void)
		{
			++_refCount;
		}
		
		void Release(void)
		{
			if (--_refCount == 0)
			{
				delete this;
			}
		}
		
		bool IsEmpty(void) const
		{
			return GetLength() == 0;
		}
	
		char GetAt(int index) const
		{
			return *(_pStr + index);
		}
	
		int GetLength(void) const
		{
			return strlen(_pStr);
		}

		const char* GetBuffer(void) const
		{
			return _pStr;
		}

		const char& operator[](int index) const
		{
			return *(_pStr + index);
		}
				
	private:
		int _refCount;
		char* _pStr;
	};

public:
	BasicString(const char* pString = nullptr)
	 : _pData(new Data(pString))
	{

	}

	BasicString(const BasicString& rhs)
	 : _pData(rhs._pData)
	{
		_pData->AddRef();
	}

	~BasicString(void)
	{
		_pData->Release();
	}

	BasicString& operator=(const BasicString& rhs)
	{
		if (this != &rhs)
		{
			_pData->Release();
			_pData = rhs._pData;
			_pData->AddRef();
		}
		return *this;
	}

	bool IsEmpty(void) const
	{
		return _pData->IsEmpty();
	}
	
	char GetAt(int index) const
	{
		return _pData->GetAt(index);
	}
	
	int GetLength(void) const
	{
		return _pData->GetLength();
	}

	const char* GetBuffer(void) const
	{
		return _pData->GetBuffer();
	}

	const char& operator[](int index) const
	{
		return (*_pData)[index];
	}

private:
	Data* _pData;
};
