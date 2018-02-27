/*
 * Vector.h
 *
 * Created: 2012/05/01 23:59:57
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace Collections
{
	template<typename value_type, typename index_type = int>
	class Vector
	{
	public:
		typedef index_type IndexType;

		Vector(index_type size)
			: _pValues(nullptr)
			, _capacity(0)
			, _used(0)
		{
			Reserve(size);
			_used = size;
		}
		
		Vector(const value_type values[], index_type size)
			: _pValues(nullptr)
			, _capacity(0)
			, _used(0)
		{
			Reserve(size);
			for (index_type i = 0; i < size; ++i)
			{
				_pValues[i] = values[i];
			}
			_used = size;
		}

		Vector(void)
			: _pValues(nullptr)
			, _capacity(0)
			, _used(0)
		{
		}

		~Vector(void)
		{
			delete[] _pValues;
		}

		void Clear(void)
		{
			delete[] _pValues;
			_capacity = 0;
			_used = 0;
		}

		bool IsEmpty(void) const
		{
			return _used == 0;
		}

		void Resize(index_type size)
		{
			Reserve(size);
			_used = size;
		}

		void Reserve(index_type size)
		{
			if (_capacity < size)
			{
				_capacity = size;
				value_type* pNewValues = new value_type[size];
				for (index_type i = 0; i < _used; ++i)
				{
					pNewValues[i] = _pValues[i];
				}
				delete[] _pValues;
				_pValues = pNewValues;
			}
		}

		value_type& operator[](index_type index)
		{
			return _pValues[index];
		}

		const value_type& operator[](index_type index) const
		{
			return _pValues[index];
		}

		value_type& GetLast(void)
		{
			return _pValues[_used - 1];
		}

		const value_type& GetLast(void) const
		{
			return _pValues[_used - 1];
		}

		value_type& GetFirst(void)
		{
			return _pValues[0];
		}

		const value_type& GetFirst(void) const
		{
			return _pValues[0];
		}

		index_type GetCount(void) const
		{
			return _used;
		}

		void Add(const value_type& value)
		{
			if (_used >= _capacity)
			{
				index_type reserveSize = ((_capacity == 0) ? 1 : _capacity) * 2;
				Reserve(reserveSize);
			}
			_pValues[_used++] = value;
		}

		bool Find(const value_type& value, index_type& findIndex)
		{
			for (index_type i = 0; i < _used; ++i)
			{
				if (_pValues[i] == value)
				{
					findIndex = i;
					return true;
				}
			}
			return false;
		}

		void RemoveAt(index_type index)
		{
			for (index_type i = index + 1; i < _used; ++i)
			{
				_pValues[i - 1] = _pValues[i];
			}
			_used--;		
		}

		bool Remove(const value_type& value)
		{
			index_type index;
			if (!Find(value, index))
			{
				return false;
			}
			RemoveAt(index);
			return true;
		}

	private:
		value_type* _pValues;
		index_type _capacity;
		index_type _used;
		
		typedef Vector<value_type, index_type> this_type;
		Vector(const this_type& rhs);
		this_type& operator=(const this_type& rhs);
	};
}
