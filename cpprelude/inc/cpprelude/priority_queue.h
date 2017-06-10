#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"
#include "cpprelude/allocator.h"
#include "cpprelude/tmp.h"
#include "algorithm.h"

namespace cpprelude {
	// Minheap is the default
	template<typename T, typename AllocatorT = global_allocator, typename Comparator = details::default_less_than<T>>
	struct priority_queue
	{
		using data_type = T;
		dynamic_array<T> _array;
		usize _count;
		Comparator _compare;

		priority_queue(Comparator compare_function = Comparator(), const AllocatorT& allocator = AllocatorT())
			:_count(0), _array(allocator), _compare(compare_function)
		{}

		priority_queue(usize count, Comparator compare_function = Comparator(), const AllocatorT& allocator = AllocatorT())
			:_count(0), _array(count, allocator), _compare(compare_function)
		{}

		priority_queue(const priority_queue& other, Comparator compare_function = Comparator(), const AllocatorT& allocator = AllocatorT())
			:_array(other._array, allocator), _count(other._count), _compare(compare_function)
		{}

		priority_queue(priority_queue&& other, Comparator compare_function, const AllocatorT& allocator)
			:_array(tmp::move(other._array), allocator), _count(other._count), _compare(compare_function)
		{}

		priority_queue(std::initializer_list<T> list, Comparator compare_function = Comparator(), const AllocatorT& allocator = AllocatorT() )
			:_count(list.size()), _array(list, allocator), _compare(compare_function)
		{
			_heapify();
		}

		template<typename iterator_type>
		priority_queue(iterator_type it, usize count, Comparator compare_function = Comparator(), const AllocatorT& allocator = AllocatorT())
			:_count(count), _array(allocator), _compare(compare_function)
		{ 
			_array.reserve(count);
			while (count--)
			{
				_array.insert_back(*it);
				it = next(it);
			}

			_heapify();
		}

		priority_queue(dynamic_array<T> arr, Comparator compare_function = Comparator(), const AllocatorT& allocator = AllocatorT())
			:_count(arr.count()), _array(arr, allocator), _compare(compare_function)
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
				_array.insert_back(tmp::move(item));
			else
				_array[_count] = tmp::move(item);

			_bubble_up(_count);
			++_count;
		}

		bool
		dequeue()
		{
			if (_count < 0) return false;
			//_array[0] = _array[_count - 1];
			tmp::swap(_array[0], _array[_count - 1]);
			--_count;
			if (_count <= _array.count() * 0.25)
			{
				_array.shrink_back(_array.count() - _count);
			}
			
			_bubble_down(0);
			return true;
		}
		
		T
		front() const
		{
			return _array[0];
		}

		T&
		front()
		{
			return _array[0];
		}
		
		void
		_heapify()
		{
			for (usize i = 0; i <_count; i++)
				_bubble_up(i);
		}

		usize
		count()
		{
			return _count;
		}

		bool
		isEmpty()
		{
			return _count == 0;
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
					tmp::swap(_array[parent], _array[index]);
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
				tmp::swap(_array[root], _array[k]);
				k = root;
			}
		}

		owner_mem_block
		decay()
		{
			_count = 0;
			return _array.decay();
		}

		owner_mem_block
		decay_continuous()
		{
			_count = 0;
			return _array.decay_continuous();
		}
	};
}