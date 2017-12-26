#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/bucket_array.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"

namespace cpprelude
{

	template<typename T,
			 usize buffer_size = details::default_size(sizeof(T))>
	struct queue_array
	{
		using data_type = T;
		bucket_array<T, buffer_size> _list;

		queue_array(memory_context* context = platform->global_memory)
			:_list(context)
		{}

		queue_array(const queue_array& other, memory_context *context)
			:_list(other._list, context)
		{}

		queue_array(queue_array&& other, memory_context *context)
			:_list(std::move(other._list), context)
		{}

		template<typename ... TArgs>
		void
		emplace(TArgs&& ... args)
		{
			_list.emplace_back(std::forward<TArgs>(args)...);
		}

		void
		enqueue(const T& item)
		{
			_list.insert_back(item);
		}

		void
		enqueue(T&& item)
		{
			_list.insert_back(std::move(item));
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

		const T&
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
