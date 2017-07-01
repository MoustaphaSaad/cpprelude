#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/slinked_list.h"
#include "cpprelude/allocator.h"

namespace cpprelude
{
	template<typename T, typename AllocatorT = global_allocator>
	struct stack_list
	{
		using data_type = T;

		slinked_list<T, AllocatorT> _list;
		
		stack_list(const AllocatorT& allocator = AllocatorT())
			:_list(allocator)
		{}

		stack_list(const stack_list&) = default;

		stack_list(stack_list&&) = default;

		stack_list(const stack_list& other, const AllocatorT& allocator)
			:_list(other._list, allocator)
		{}

		stack_list(stack_list&& other, const AllocatorT& allocator)
			:_list(tmp::move(other), allocator)
		{}

		void
		push(const T& item)
		{
			_list.insert_front(item);
		}

		void
		push(T&& item)
		{
			_list.insert_front(tmp::move(item));
		}

		T
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

		slice<typename slinked_list<T, AllocatorT>::node_type>
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
