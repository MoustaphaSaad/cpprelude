#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/bucket_array.h"
#include "cpprelude/allocator.h"
#include "cpprelude/tmp.h"

namespace cpprelude
{

	template<typename T,
			 usize buffer_size = details::default_size(sizeof(T)),
			 typename AllocatorT = global_allocator>
	struct queue_array
	{
		using data_type = T;
		bucket_array<T, buffer_size, AllocatorT> _list;

		queue_array(const AllocatorT& allocator = AllocatorT())
			:_list(allocator)
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

		slice<T>
		decay()
		{
			return decay_continuous();
		}

		slice<T>
		decay_continuous()
		{
			return _list.decay_continuous();
		}
	};
}
