#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/slinked_list.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"

namespace cpprelude
{
	template<typename T>
	struct stack_list
	{
		using data_type = T;

		slinked_list<T> _list;

		stack_list(memory_context* context = platform.global_memory)
			:_list(context)
		{}

		stack_list(const stack_list&) = default;

		stack_list(stack_list&&) = default;

		stack_list(const stack_list& other, memory_context* context)
			:_list(other._list, context)
		{}

		stack_list(stack_list&& other, memory_context* context)
			:_list(std::move(other), context)
		{}

		template<typename ... TArgs>
		void
		emplace(TArgs&& ... args)
		{
			_list.emplace_front(std::forward<TArgs>(args)...);
		}

		void
		push(const T& item)
		{
			_list.insert_front(item);
		}

		void
		push(T&& item)
		{
			_list.insert_front(std::move(item));
		}

		const T&
		top() const
		{
			return *_list.begin();
		}

		T&
		top()
		{
			return *_list.begin();
		}

		bool
		pop()
		{
			if (_list.count() > 0)
			{
				_list.remove_front();
				return true;
			}

			return false;
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

		slice<typename slinked_list<T>::node_type>
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
