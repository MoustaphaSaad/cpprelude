#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dlinked_list.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"

namespace cpprelude
{
	template<typename T>
	struct queue_list
	{
		using data_type = T;

		dlinked_list<T> _list;

		queue_list(memory_context* context = platform.global_memory)
			:_list(context)
		{}

		queue_list(const queue_list&) = default;

		queue_list(queue_list&&) = default;

		queue_list(const queue_list& other, memory_context* context)
			:_list(other._list, context)
		{}

		queue_list(queue_list&& other, memory_context* context)
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

		slice<typename dlinked_list<T>::node_type>
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
