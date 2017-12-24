#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
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

		API_CPPR arena_t(usize size, bool use_virtual_memory = true);
		API_CPPR ~arena_t();

		API_CPPR memory_context*
		context();

		inline
		operator memory_context*()
		{
			return &_context;
		}

		API_CPPR void
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
