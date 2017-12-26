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
		memory_context* _context = platform->global_memory;
		io_trait _io_trait;

		API_CPPR memory_stream(memory_context *context = platform->global_memory);

		API_CPPR memory_stream(const slice<byte>& data, memory_context *context = platform->global_memory);

		API_CPPR memory_stream(slice<byte>&& data, memory_context *context = platform->global_memory);

		memory_stream(const memory_stream&) = delete;

		memory_stream&
		operator=(const memory_stream&) = delete;

		API_CPPR memory_stream(memory_stream&& other);

		API_CPPR memory_stream(memory_stream&& other, memory_context *context);

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

		API_CPPR usize
		write(const slice<byte>& data);

		API_CPPR usize
		read(slice<byte>& data);

		API_CPPR usize
		read(slice<byte>&& data);
	};
}
