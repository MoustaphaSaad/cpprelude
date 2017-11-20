#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/platform.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/string.h"
#include <cstdio>

namespace cpprelude
{
	//traits
	struct stream_trait
	{
		using write_func = usize(*)(void*, const slice<byte>&);
		using read_func = usize(*)(void*, slice<byte>&);

		void* _self = nullptr;
		write_func _write = nullptr;
		read_func _read = nullptr;

		API usize
		write(const slice<byte>& data);

		API usize
		read(slice<byte>& data);

		API usize
		read(slice<byte>&& data);
	};

	//function interface
	template<typename T>
	inline static usize
	write(stream_trait* stream, const T& value)
	{
		return stream->write(make_slice((byte*)&value, sizeof(T)));
	}

	template<typename T>
	inline static usize
	write(stream_trait* stream, const slice<T>& values)
	{
		return stream->write(values.template convert<byte>());
	}

	template<typename T>
	inline static usize
	write_str(stream_trait* stream, const T& value)
	{
		static_assert(sizeof(T) == 0, "[write_str]: is not implemented for this type");
		return 0;
	}

	template<typename T>
	inline static usize
	read(stream_trait* stream, T& value)
	{
		return stream->read(make_slice((byte*)&value, sizeof(T)));
	}

	template<typename T>
	inline static usize
	read(stream_trait* stream, slice<T>& values)
	{
		return stream->read(values.template convert<byte>());
	}

	template<typename T>
	inline static usize
	read(stream_trait* stream, slice<T>&& values)
	{
		return read(stream, values);
	}

//implement the string stuff
#define DEFINE_WRITE_STR(PATTERN, TYPE, BUFFER_SIZE)\
	inline static usize\
	write_str(stream_trait* stream, TYPE value)\
	{\
		byte buffer[BUFFER_SIZE];\
		int written_size = std::snprintf(buffer, BUFFER_SIZE, PATTERN, value);\
		if(written_size < 0) return 0;\
		return stream->write(make_slice<byte>(buffer, written_size));\
	}

	DEFINE_WRITE_STR("%c", byte, 6)

	DEFINE_WRITE_STR("%hhd", i8, 6)
	DEFINE_WRITE_STR("%hhu", u8, 6)

	DEFINE_WRITE_STR("%hd", i16, 10)
	DEFINE_WRITE_STR("%hu", u16, 10)

	DEFINE_WRITE_STR("%d", i32, 20)
	DEFINE_WRITE_STR("%u", u32, 20)

	DEFINE_WRITE_STR("%ld", i64, 40)
	DEFINE_WRITE_STR("%lu", u64, 40)

	DEFINE_WRITE_STR("%f", r32, 128)
	DEFINE_WRITE_STR("%f", r64, 128)

	DEFINE_WRITE_STR("%p", void*, 40)
#undef DEFINE_WRITE_STR

	//string stuff
	inline static usize
	write(stream_trait* stream, const cpprelude::string& str)
	{
		return stream->write(str._data);
	}

	inline static usize
	read(stream_trait* stream, cpprelude::string& str)
	{
		return stream->read(str._data);
	}

	inline static usize
	write_str(stream_trait* stream, const cpprelude::string& str)
	{
		auto str_view = make_slice(str._data.ptr, str.count());
		return stream->write(str_view);
	}

	//raw string stuff
	inline static usize
	write_str(stream_trait* stream, const char* str)
	{
		auto str_view = make_slice((byte*)str, strlen(str));
		return stream->write(str_view);
	}

	//variadic templates for convience
	inline static usize
	vwrite(stream_trait* stream)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vwrite(stream_trait* stream, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += write(stream, std::forward<TFirst>(first_arg));
		result += vwrite(stream, std::forward<TArgs>(args)...);
		return result;
	}

	inline static usize
	vwrite_str(stream_trait* stream)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vwrite_str(stream_trait* stream, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += write_str(stream, std::forward<TFirst>(first_arg));
		result += vwrite_str(stream, std::forward<TArgs>(args)...);
		return result;
	}

	inline static usize
	vread(stream_trait* stream)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vread(stream_trait* stream, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += read(stream, std::forward<TFirst>(first_arg));
		result += vread(stream, std::forward<TArgs>(args)...);
		return result;
	}

	//memory stream
	struct memory_stream
	{
		slice<byte> _data;
		usize _read_head = 0, _write_head = 0;
		memory_context* _context = platform.global_memory;
		stream_trait _trait;

		API memory_stream();
		API ~memory_stream();

		memory_stream(const memory_stream&) = delete;

		memory_stream&
		operator=(const memory_stream&) = delete;

		API memory_stream(memory_stream&& other);

		API memory_stream&
		operator=(memory_stream&& other);

		API operator stream_trait*();

		API usize
		size() const;

		API usize
		capacity() const;

		API usize
		write_capacity() const;

		API usize
		read_position() const;

		API usize
		read_capacity() const;

		API bool
		empty() const;

		API void
		clear();

		API slice<byte>
		decay();

		API slice<byte>
		decay_continuous();

		API string
		string_decay();
	};

	template<typename ... TArgs>
	inline static string
	concat(TArgs&& ... args)
	{
		memory_stream result;
		vwrite_str(result, std::forward<TArgs>(args)...);
		return result.string_decay();
	}

	enum class IO_MODE
	{
		NONE, READ, WRITE, APPEND, READ_EXTENDED, WRITE_EXTENDED, APPEND_EXTENDED
	};

	//file stream
	struct file_stream
	{
		FILE* _handle = nullptr;
		usize _read_head = 0, _write_head = 0, _cursor_position = 0;
		IO_MODE mode;
		string name;
		stream_trait _trait;

		API file_stream();
		API ~file_stream();

		file_stream(const file_stream&) = delete;

		file_stream&
		operator=(const file_stream&) = delete;

		API file_stream(file_stream&& other);

		API file_stream&
		operator=(file_stream&& other);

		API operator stream_trait*();

		API bool
		valid() const;

		API usize
		size() const;

		API usize
		read_position() const;
	};

	API file_stream
	open_file(const string& filename, IO_MODE openmode, bool binary = true);

	API file_stream
	open_file(string&& filename, IO_MODE openmode, bool binary = true);
}