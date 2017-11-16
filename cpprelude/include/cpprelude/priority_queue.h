#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"
#include "cpprelude/defaults.h"

namespace cpprelude {
	// Minheap is the default
	template<typename T, typename Comparator = default_less_than<T>>
	struct priority_queue
	{
		using data_type = T;
		dynamic_array<T> _array;
		usize _count;
		Comparator _compare;

		priority_queue(Comparator compare_function = Comparator(), memory_context* context = platform.global_memory)
			:_array(context), _count(0), _compare(compare_function)
		{}

		priority_queue(usize count, Comparator compare_function = Comparator(), memory_context* context = platform.global_memory)
			:_array(count, context), _count(0), _compare(compare_function)
		{}

		priority_queue(const priority_queue& other, Comparator compare_function = Comparator(), memory_context* context = platform.global_memory)
			:_array(other._array, context), _count(other._count), _compare(compare_function)
		{}

		priority_queue(priority_queue&& other, Comparator compare_function, memory_context* context)
			:_array(std::move(other._array), context), _count(other._count), _compare(compare_function)
		{}

		priority_queue(std::initializer_list<T> list, Comparator compare_function = Comparator(), memory_context* context = platform.global_memory)
			:_array(list, context), _count(list.size()), _compare(compare_function)
		{
			_heapify();
		}

		template<typename iterator_type>
		priority_queue(iterator_type it, usize count, Comparator compare_function = Comparator(), memory_context* context = platform.global_memory)
			:_array(context), _count(count), _compare(compare_function)
		{ 
			_array.reserve(count);
			while (count--)
			{
				_array.insert_back(*it);
				it = next(it);
			}

			_heapify();
		}

		priority_queue(const dynamic_array<T>& arr, Comparator compare_function = Comparator(), memory_context* context = platform.global_memory)
			:_array(arr, context), _count(arr.count()), _compare(compare_function)
		{
			_heapify();
		}
		
		void
		enqueue(const T& item)
		{
			if (_count == _array.count())
				_array.insert_back(item);
			else
				_array[_count] = item;

			_bubble_up(_count);
			++_count;
		}
		
		void
		enqueue(T&& item)
		{
			if (_count == _array.count())
				_array.insert_back(std::move(item));
			else
				_array[_count] = std::move(item);

			_bubble_up(_count);
			++_count;
		}

		bool
		dequeue()
		{
			if (_count == 0) return false;
			std::swap(_array[0], _array[_count - 1]);
			--_count;
			if (_count <= _array.count() * 0.25)
			{
				_array.shrink_back(_array.count() - _count);
			}
			
			_bubble_down(0);
			return true;
		}
		
		const T&
		front() const
		{
			return _array[0];
		}

		T&
		front()
		{
			return _array[0];
		}

		usize
		count() const
		{
			return _count;
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		void
		_heapify()
		{
			for (usize i = 0; i <_count; i++)
				_bubble_up(i);
		}

		void
		_bubble_up(usize index)
		{
			while (index > 0)
			{
				usize parent = (index - 1) / 2;
				//which index has the highest priority item
				if (!_compare(_array[parent], _array[index]))
				{
					std::swap(_array[parent], _array[index]);
					index = parent;
				}
				else break;
			}
		}

		void
		_bubble_down(usize k)
		{
			while (true)
			{
				usize l = 2 * k + 1;
				usize r = 2 * k + 2;
				usize root = k;
				//which index has the highest priority item
				if (l < _count && !_compare(_array[root], _array[l]))
					root = l;
				if (r < _count && !_compare(_array[root], _array[r]))
					root = r;
				
				//If it is the same index then there is no violation for heap's rules
				if (root == k) break;
				//swap
				std::swap(_array[root], _array[k]);
				k = root;
			}
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
