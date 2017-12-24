#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/platform.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/io.h"

namespace cpprelude
{
	struct memory_stream
	{
		slice<byte> _data;
		usize _cursor = 0, _size = 0;
		memory_context* _context = platform.global_memory;
		io_trait _io_trait;

		API_CPPR memory_stream();

		API_CPPR memory_stream(const slice<byte>& data, memory_context *context = platform.global_memory);

		API_CPPR memory_stream(slice<byte>&& data, memory_context *context = platform.global_memory);

		memory_stream(const memory_stream&) = delete;

		memory_stream&
		operator=(const memory_stream&) = delete;

		API_CPPR memory_stream(memory_stream&& other);

		API_CPPR memory_stream&
		operator=(memory_stream&& other);

		API_CPPR ~memory_stream();

		inline
		operator io_trait*()
		{
			return &_io_trait;
		}

		API_CPPR usize
		size() const;

		API_CPPR usize
		cursor() const;

		API_CPPR bool
		move(isize offset);

		API_CPPR bool
		move_to_start();

		API_CPPR bool
		move_to_end();

		API_CPPR bool
		empty() const;

		API_CPPR void
		clear();

		API_CPPR void
		reset();

		API_CPPR usize
		capacity() const;

		API_CPPR void
		reserve(usize expected_size);

		API_CPPR slice<byte>
		decay();

		API_CPPR slice<byte>
		decay_continuous();
	};
	
	//memory stream
	/*struct memory_stream
	{
		slice<byte> _data;
		usize _read_head = 0, _write_head = 0;
		memory_context* _context = platform.global_memory;
		stream_trait _trait;

		API_CPPR memory_stream();
		API_CPPR ~memory_stream();

		memory_stream(const memory_stream&) = delete;

		memory_stream&
		operator=(const memory_stream&) = delete;

		API_CPPR memory_stream(memory_stream&& other);

		API_CPPR memory_stream&
		operator=(memory_stream&& other);

		API_CPPR operator stream_trait*();

		API_CPPR usize
		size() const;

		API_CPPR usize
		capacity() const;

		API_CPPR usize
		write_capacity() const;

		API_CPPR usize
		read_position() const;

		API_CPPR usize
		read_capacity() const;

		API_CPPR bool
		empty() const;

		API_CPPR void
		clear();

		API_CPPR slice<byte>
		decay();

		API_CPPR slice<byte>
		decay_continuous();

		API_CPPR string
		string_decay();
	};*/

	/*template<typename ... TArgs>
	inline static string
	concat(TArgs&& ... args)
	{
		memory_stream result;
		vwrite_str(result, std::forward<TArgs>(args)...);
		return result.string_decay();
	}*/
}