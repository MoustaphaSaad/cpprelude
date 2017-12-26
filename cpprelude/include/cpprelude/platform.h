#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/memory.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/file_defs.h"
#include "cpprelude/heap.h"
#include "cpprelude/result.h"

namespace cpprelude
{
	struct string;

	enum class PLATFORM_ERROR
	{
		OK,
		GENERIC_ERROR,
		FILE_ALREADY_EXISTS,
		FILE_DOESNOT_EXIST
	};

	struct platform_t
	{
		//heap_t heap;
		memory_context* global_memory;
		usize allocation_count = 0;
		usize allocation_size = 0;
		usize RAM_SIZE;
		bool debug_configured = false;

		~platform_t();

		API_CPPR slice<byte>
		virtual_alloc(void* address_hint, usize size);

		API_CPPR bool
		virtual_free(slice<byte>& data);

		API_CPPR bool
		virtual_free(slice<byte>&& data);

		template<typename T>
		slice<T>
		alloc(usize count = 1)
		{
			return global_memory->template alloc<T>(count);
		}

		template<typename T>
		void
		free(slice<T>& data)
		{
			global_memory->free(data);
		}

		template<typename T>
		void
		free(slice<T>&& data)
		{
			global_memory->free(data);
		}

		template<typename T>
		void
		realloc(slice<T>& data, usize count)
		{
			global_memory->realloc(data, count);
		}

		template<typename T>
		void
		realloc(slice<T>&& data, usize count)
		{
			global_memory->realloc(data, count);
		}

		API_CPPR void
		print_memory_report() const;

		API_CPPR void
		dump_callstack() const;

		API_CPPR result<file_handle, PLATFORM_ERROR>
		file_open(const string& filename,
			IO_MODE io_mode = IO_MODE::READ_WRITE,
			OPEN_MODE open_mode = OPEN_MODE::CREATE_OVERWRITE);

		API_CPPR bool
		file_close(file_handle& handle);

		API_CPPR bool
		file_close(file_handle&& handle);

		API_CPPR bool
		file_valid(const file_handle& handle);

		API_CPPR usize
		file_write(const file_handle& handle, const slice<byte>& data);

		API_CPPR usize
		file_read(const file_handle& handle, slice<byte>& data);

		API_CPPR usize
		file_read(const file_handle& handle, slice<byte>&& data);

		API_CPPR i64
		file_size(const file_handle& handle);

		API_CPPR i64
		file_cursor(const file_handle& handle);

		API_CPPR bool
		file_move(const file_handle& handle, i64 offset);

		API_CPPR bool
		file_move_to_start(const file_handle& handle);

		API_CPPR bool
		file_move_to_end(const file_handle& handle);
	};

	API_CPPR void
	_init_platform();
	
	API_CPPR extern platform_t* platform;
}