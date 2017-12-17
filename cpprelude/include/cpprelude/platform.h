#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/memory.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/file_defs.h"
#include "cpprelude/heap.h"
#include "cpprelude/result.h"

#include <iostream>

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
		open_file(const string& filename,
			IO_MODE2 io_mode = IO_MODE2::READ_WRITE,
			OPEN_MODE open_mode = OPEN_MODE::CREATE_OVERWRITE);

		API_CPPR bool
		close_file(file_handle& handle);

		API_CPPR bool
		close_file(file_handle&& handle);

		API_CPPR bool
		valid_file(const file_handle& handle);

		API_CPPR usize
		write_file(const file_handle& handle, const slice<byte>& data);

		API_CPPR usize
		read_file(const file_handle& handle, slice<byte>& data);

		API_CPPR usize
		read_file(const file_handle& handle, slice<byte>&& data);

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

	API_CPPR extern platform_t& platform;

	//print stuff

	namespace helper
	{
		API_CPPR void
		__acquire_print_lock();

		API_CPPR void
		__release_print_lock();

		template<typename TStream>
		inline static void
		_print(TStream& out)
		{}

		template<typename TStream, typename TFirst, typename ... TArgs>
		inline static void
		_print(TStream& out, TFirst&& arg, TArgs&& ... args)
		{
			out << arg;
			_print(out, std::forward<TArgs>(args)...);
		}
	}

	template<typename TStream, typename ... TArgs>
	inline static void
	print(TStream& out, TArgs&& ... args)
	{
		helper::__acquire_print_lock();
		helper::_print(out, std::forward<TArgs>(args)...);
		helper::__release_print_lock();
	}

	template<typename TStream, typename ... TArgs>
	inline static void
	println(TStream& out, TArgs&& ... args)
	{
		helper::__acquire_print_lock();
		helper::_print(out, std::forward<TArgs>(args)..., "\n");
		helper::__release_print_lock();
	}
}