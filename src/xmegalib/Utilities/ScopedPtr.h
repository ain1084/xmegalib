/*
 * ScopedPtr.h
 *
 * Created: 2012/05/07 0:22:20
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

template<typename T>
class ScopedPtr
{
public:
	ScopedPtr(void)
	: _p(nullptr)
	{
		
	}

	ScopedPtr(T* p)
	: _p(p)
	{
		
	}
	
	~ScopedPtr(void)
	{
		delete _p;
	}
	
	void Reset(T* p = nullptr)
	{
		delete _p;
		_p = p;
	}
	
	T* operator->() const
	{
		return _p;
	}

	T& operator*() const
	{
		return *_p;
	}

	T* Get(void) const
	{
		return _p;
	}
	
	T* Release(void)
	{
		T* result = _p;
		_p = nullptr;
		return result;
	}
	
	operator bool() const
	{
		return _p != nullptr;
	}

private:
	ScopedPtr(const ScopedPtr<T>&) = delete;
	ScopedPtr<T>& operator=(const ScopedPtr<T>&) = delete;

	T* _p;
};