#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"

namespace cpprelude
{
	struct arena_t
	{
		slice<byte> _memory;
		usize _allocation_head;
		memory_context _context;
		bool _uses_virtual_memory;

		API arena_t(usize size, bool use_virtual_memory = true);
		API ~arena_t();

		API memory_context*
		context();

		API void
		free_all();

		template<typename T>
		slice<T>
		alloc(usize count = 1)
		{
			return _context.template alloc<T>(count);
		}

		template<typename T>
		void
		free(slice<T>& data)
		{
			_context.template free<T>(data);
		}

		template<typename T>
		void
		free(slice<T>&& data)
		{
			_context.template free<T>(data);
		}

		template<typename T>
		void
		realloc(slice<T>& data, usize count)
		{
			_context.template realloc<T>(data, count);
		}

		template<typename T>
		void
		realloc(slice<T>&& data, usize count)
		{
			_context.template realloc<T>(data, count);
		}
	};
}
