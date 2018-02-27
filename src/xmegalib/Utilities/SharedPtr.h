/*
 * SharedPtr.h
 *
 * Created: 2012/06/04 0:32:04
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "ScopedPtr.h"

template<typename T>
class SharedPtr
{
private:
	class Data
	{
	public:
		Data(T* pObject)
		 : _pObject(pObject)
		 , _refCount(1)
		{
			 
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
		
		const ScopedPtr<T>& GetObject(void) const
		{
			return _pObject; 
		}
	private:
		ScopedPtr<T> _pObject;
		unsigned _refCount;
	};
	
	Data *_pData;

public:
	explicit SharedPtr(T* p = nullptr)
	{
		if (p != nullptr)
		{
			_pData = new Data(p);
		}
		else
		{
			_pData = nullptr;
		}
	}

	explicit SharedPtr(const SharedPtr& rhs)
	{
		if (rhs._pData != nullptr)
		{
			_pData = rhs._pData;
			_pData->AddRef();
		}
		else
		{
			_pData = nullptr;
		}
	}

	SharedPtr& operator=(const SharedPtr& rhs)
	{
		if (this != &rhs)
		{
			if (_pData != nullptr)
			{
				_pData->Release();
				_pData = nullptr;
			}
			if (rhs._pData != nullptr)
			{
				_pData = rhs._pData;
				_pData->AddRef();
			}
		}
		return *this;
	}

	~SharedPtr(void)
	{
		if (_pData != nullptr)
		{
			_pData->Release();
		}
	}	

	T* operator->() const
	{
		return Get();
	}

	T* Get(void) const
	{
		if (_pData != nullptr)
		{
			return _pData->GetObject().Get();
		}
		else
		{
			return nullptr;
		}		
	}
	
	operator bool() const
	{
		if (_pData != nullptr)
		{
			return _pData->GetObject();
		}
		else
		{
			return false;
		}
	}
	
	bool operator==(const SharedPtr<T>& rhs) const
	{
		return rhs._pData == _pData;
	}
	
	bool operator!=(const SharedPtr<T>& rhs) const
	{
		return rhs._pData != _pData;
	}
};
