/*
 * Stack.h
 *
 * Created: 2012/05/07 9:37:42
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Vector.h"

namespace Collections
{
	template<typename value_type, typename index_type = int>
	class Stack
	{
	public:
		Stack(void)
		{
			
		}
	
		void Push(const value_type& value)
		{
			_container.Add(value);
		}
		
		bool IsEmpty(void) const
		{
			return _container.IsEmpty();
		}
		
		void Clear(void)
		{
			_container.Clear();
		}
		
		index_type GetCount(void) const
		{
			return _container.GetCount();
		}
		
		void Pop(void)
		{
			if (_container.IsEmpty())
			{
				return;
			}
			_container.RemoveAt(_container.GetCount() - 1);
		}
		
		value_type& GetTop(void)
		{
			return _container.GetLast();
		}
		
		const value_type& GetTop(void) const
		{
			return _container.GetLast();
		}

	private:
		Vector<value_type, index_type> _container;

		typedef Stack<value_type, index_type> this_type;
		Stack(const this_type& rhs);
		this_type& operator=(const this_type& rhs);
	};
}
