#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dlinked_list.h"
#include "cpprelude/allocator.h"

namespace cpprelude
{
	template<typename T, typename AllocatorT = global_allocator>
	struct queue_list
	{
		using data_type = T;

		dlinked_list<T, AllocatorT> _list;

		queue_list(const AllocatorT& allocator = AllocatorT())
			:_list(allocator)
		{}

		queue_list(const queue_list&) = default;

		queue_list(queue_list&&) = default;

		queue_list(const queue_list& other, const AllocatorT& allocator)
			:_list(other._list, allocator)
		{}

		queue_list(queue_list&& other, const AllocatorT& allocator)
			:_list(tmp::move(other._list), allocator)
		{}

		void
		enqueue(const T& item)
		{
			_list.insert_back(item);
		}

		void
		enqueue(T&& item)
		{
			_list.insert_back(tmp::move(item));
		}

		bool
		dequeue()
		{
			if (!_list.empty())
			{
				_list.remove_front();
				return true;
			}

			return false;
		}

		T
		front() const
		{
			return *_list.front();
		}

		T&
		front() 
		{
			return *_list.front();
		}

		bool
		empty() const
		{
			return _list.empty();
		}

		usize
		count() const
		{
			return _list.count();
		}

		slice<typename dlinked_list<T, AllocatorT>::node_type>
		decay()
		{
			return _list.decay();
		}

		slice<T>
		decay_continuous()
		{
			return _list.decay_continuous();
		}
	};
}
