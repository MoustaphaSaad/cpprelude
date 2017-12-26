#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"

namespace cpprelude
{
	template<typename T>
	struct stack_array
	{
		using data_type = T;

		dynamic_array<T> _array;
		usize _count;

		stack_array(memory_context* context = platform->global_memory)
			: _array(context), _count(0)
		{}

		stack_array(const stack_array&) = default;

		stack_array(stack_array&&) = default;

		stack_array(usize count, memory_context* context = platform->global_memory)
			:_array(count, context), _count(0)
		{}

		stack_array(const stack_array& other, memory_context* context)
			:_array(other._array, context), _count(other._count)
		{}

		stack_array(stack_array&& other, memory_context* context)
			:_array(std::move(other._array), context), _count(other._count)
		{}

		template<typename ... TArgs>
		void
		emplace(TArgs&& ... args)
		{
			if (_count == _array.count())
				_array.emplace_back(std::forward<TArgs>(args)...);
			else
				new (_array.data() + _count) T(std::forward<TArgs>(args)...);

			++_count;
		}

		void
		push(const T& item)
		{
			if (_count == _array.count())
				_array.insert_back(item);
			else
				new (_array.data() + _count) T(item);

			++_count;
		}

		void
		push(T&& item)
		{
			if (_count == _array.count())
				_array.insert_back(std::move(item));
			else
				new (_array.data() + _count) T(std::move(item));

			++_count;
		}

		const T&
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
