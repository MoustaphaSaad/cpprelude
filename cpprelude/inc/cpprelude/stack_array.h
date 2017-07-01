#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"
#include "cpprelude/allocator.h"

namespace cpprelude
{
	template<typename T, typename AllocatorT = global_allocator>
	struct stack_array
	{
		using data_type = T;

		dynamic_array<T, AllocatorT> _array;
		usize _count;

		stack_array(const AllocatorT& allocator = AllocatorT())
			:_count(0), _array(allocator)
		{}

		stack_array(const stack_array&) = default;

		stack_array(stack_array&&) = default;

		stack_array(usize count, const AllocatorT& allocator = AllocatorT())
			:_count(0), _array(count, allocator)
		{}

		stack_array(const stack_array& other, const AllocatorT& allocator)
			:_array(other._array, allocator), _count(other._count)
		{}

		stack_array(stack_array&& other, const AllocatorT& allocator)
			:_array(tmp::move(other._array), allocator), _count(other._count)
		{}

		void
		push(const T& item)
		{
			if (_count == _array.count())
				_array.insert_back(item);
			else
				_array[_count] = item;

			++_count;
		}

		void
		push(T&& item)
		{
			if (_count == _array.count())
				_array.insert_back(tmp::move(item));
			else
				_array[_count] = tmp::move(item);

			++_count;
		}
		
		T
		top() const
		{
			return _array[_count-1];
		}

		T&
		top()
		{
			return _array[_count-1];
		}

		bool
		pop()
		{
			if(_count > 0)
			{
				--_count;
				if(_count <= _array.count() * 0.25)
				{
					_array.shrink_back(_array.count() - _count);
				}
				return true;
			}

			return false;
		}

		bool
		empty() const
		{
			return (_count == 0);
		}

		usize
		count() const
		{
			return _count;
		}

		slice<T>
		decay()
		{
			_count = 0;
			return _array.decay();
		}

		slice<T>
		decay_continuous()
		{
			_count = 0;
			return _array.decay_continuous();
		}

	};

}
