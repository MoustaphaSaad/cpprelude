#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/memory.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"
#include "cpprelude/io.h"
#include "cpprelude/stream.h"

namespace cpprelude
{
	//buffered input trait
	struct bufin_trait
	{
		//peeks at the in coming buffer you don't need to explicitly manage the returned slice
		using peek_func = slice<byte>(*)(void*, usize);

		//moves the buffer forward
		using skip_func = usize(*)(void*, usize);

		//returns the size of available buffered data
		using available_func = usize(*)(void*);

		void *_self = nullptr;
		peek_func _peek = nullptr;
		skip_func _skip = nullptr;
		available_func _available = nullptr;

		//peek into the buffer. if size == 0 then it will just return the available buffer size
		//else it will attempt to load the requested size
		slice<byte>
		peek(usize size = 0)
		{
			return _peek(_self, size);
		}

		//skip the requested size
		usize
		skip(usize size)
		{
			return _skip(_self, size);
		}

		//returns the amount of the buffered data
		usize
		available()
		{
			return _available(_self);
		}

		//attempts to read data
		usize
		read(slice<byte>& data)
		{
			auto result = peek(data.size);
			copy_slice<byte>(data, result, result.size);
			skip(result.size);
			return result.size;
		}

		usize
		read(slice<byte>&& data)
		{
			return read(data);
		}
	};

	struct buf_reader
	{
		memory_context *_context = platform->global_memory;
		memory_stream _buffer;
		io_trait *_input_io = nullptr;
		bufin_trait _bufin_trait;

		API_CPPR buf_reader(io_trait *trait, memory_context *context = platform->global_memory);

		buf_reader(const buf_reader&) = delete;

		API_CPPR buf_reader(buf_reader&& other);

		API_CPPR buf_reader(buf_reader&& other, memory_context *context);

		buf_reader&
		operator=(const buf_reader&) = delete;

		API_CPPR buf_reader&
		operator=(buf_reader&& other);

		inline
		operator bufin_trait*()
		{
			return &_bufin_trait;
		}

		API_CPPR slice<byte>
		peek(usize size = 0);

		API_CPPR usize
		skip(usize size);

		API_CPPR usize
		available();

		API_CPPR usize
		read(slice<byte>& data);

		API_CPPR usize
		read(slice<byte>&& data);
	};


	//buffered writer
	struct bufout_trait
	{
		using write_func = usize(*)(void*, const slice<byte>&);
		//returns the amount flushed
		using flush_func = usize(*)(void*);
		//returns the amount of the data buffered
		using available_func = usize(*)(void*);

		void *_self = nullptr;
		write_func _write = nullptr;
		flush_func _flush = nullptr;
		available_func _available = nullptr;

		usize
		write(const slice<byte>& data)
		{
			return _write(_self, data);
		}

		usize
		flush()
		{
			return _flush(_self);
		}

		usize
		available()
		{
			return _available(_self);
		}
	};

	struct buf_writer
	{
		memory_context *_context = platform->global_memory;
		memory_stream _buffer;
		io_trait *_output_io = nullptr;
		bufout_trait _bufout_trait;

		API_CPPR buf_writer(io_trait *trait, memory_context *context = platform->global_memory);

		buf_writer(const buf_writer&) = delete;

		API_CPPR buf_writer(buf_writer&& other);

		API_CPPR buf_writer(buf_writer&& other, memory_context *context);

		API_CPPR ~buf_writer();

		buf_writer&
		operator=(const buf_writer&) = delete;

		API_CPPR buf_writer&
		operator=(buf_writer&& other);

		inline
		operator bufout_trait*()
		{
			return &_bufout_trait;
		}

		API_CPPR usize
		write(const slice<byte>& data);

		API_CPPR usize
		flush();

		API_CPPR usize
		available();
	};

	API_CPPR extern bufin_trait* buf_stdin;
	API_CPPR extern bufout_trait* buf_stdout;
	API_CPPR extern bufout_trait* buf_stderr;
}