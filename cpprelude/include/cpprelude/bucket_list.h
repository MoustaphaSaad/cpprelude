#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"

namespace cpprelude
{
	template<typename T,
			 usize bucket_size = 128>
	struct bucket_list
	{
		using data_type = T;
		
		slice<T> _memory;
		memory_context *_context = platform.global_memory;
		bucket_list* _next;

		bucket_list(memory_context *context = platform.global_memory)
			:_context(context), _next(nullptr)
		{
			_memory = _context->template alloc<T>(bucket_size);
		}

		bucket_list(const bucket_list&) = delete;
		
		bucket_list&
		operator=(const bucket_list&) = delete;

		~bucket_list()
		{
			if(_memory.ptr != nullptr && _memory.size > 0)
				if(_context) _context->free(_memory);

			if(_next)
			{
				_next->~bucket_list();
				if(_context) _context->free(make_slice(_next));
			}
		}

		T&
		operator[](usize index)
		{
			if(index >= bucket_size)
			{
				auto it = _next;
				index -= bucket_size;
				while(index >= bucket_size)
				{
					it = it->_next;
					index -= bucket_size;
				}
				return it->_memory[index];
			}

			return _memory[index];
		}

		const T&
		operator[](usize index) const
		{
			if(index >= bucket_size)
			{
				auto it = _next;
				index -= bucket_size;
				while(index >= bucket_size)
				{
					it = it->_next;
					index -= bucket_size;
				}
				return it->_memory[index];
			}

			return _memory[index];
		}

		T*
		at(usize index)
		{
			if(index >= bucket_size)
			{
				auto it = _next;
				index -= bucket_size;
				while(index >= bucket_size)
				{
					it = it->_next;
					index -= bucket_size;
				}
				return &it->_memory[index];
			}

			return &_memory[index];
		}

		void
		expand()
		{
			if(_next)
				_next->expand();
			else
			{
				_next = _context->template alloc<bucket_list>();
				new (_next) bucket_list(_context);
			}
		}
	};
}
