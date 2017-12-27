#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/memory.h"
#include "cpprelude/io.h"
#include "cpprelude/bufio.h"
#include "cpprelude/stream.h"
#include "cpprelude/string.h"

namespace cpprelude
{
	//section(print)
	//section(print_bin)
	template<typename T>
	inline static usize
	print_bin(io_trait *trait, const T& value)
	{
		return trait->write(make_slice((byte*)&value, sizeof(T)));
	}

	template<typename T>
	inline static usize
	print_bin(bufout_trait *trait, const T& value)
	{
		return trait->write(make_slice((byte*)&value, sizeof(T)));
	}


	template<typename T>
	inline static usize
	print_bin(io_trait *trait, const slice<T>& values)
	{
		return trait->write(values.template convert<byte>());
	}

	template<typename T>
	inline static usize
	print_bin(bufout_trait *trait, const slice<T>& values)
	{
		return trait->write(values.template convert<byte>());
	}


	//section(vprintb)
	inline static usize
	vprintb(io_trait *trait)
	{
		return 0;
	}

	inline static usize
	vprintb(bufout_trait *trait)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vprintb(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += print_bin(trait, std::forward<TFirst>(first_arg));
		result += vprintb(trait, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vprintb(bufout_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += print_bin(trait, std::forward<TFirst>(first_arg));
		result += vprintb(trait, std::forward<TArgs>(args)...);
		return result;
	}


	//section(print_str)
	#define DEFINE_PRINT_STR(TRAIT, TYPE, PATTERN, BUFFER_SIZE)\
	inline static usize\
	print_str(TRAIT *trait, TYPE value)\
	{\
		byte buffer[BUFFER_SIZE];\
		int written_size = std::snprintf(buffer, BUFFER_SIZE, PATTERN, value);\
		if(written_size < 0) return 0;\
		return trait->write(make_slice<byte>(buffer, written_size));\
	}

	DEFINE_PRINT_STR(io_trait, byte, "%c", 6)
	DEFINE_PRINT_STR(bufout_trait, byte, "%c", 6)

	DEFINE_PRINT_STR(io_trait, i8, "%hhd", 6)
	DEFINE_PRINT_STR(bufout_trait, i8, "%hhd", 6)

	DEFINE_PRINT_STR(io_trait, u8, "%hhu", 6)
	DEFINE_PRINT_STR(bufout_trait, u8, "%hhu", 6)

	DEFINE_PRINT_STR(io_trait, i16, "%hd", 10)
	DEFINE_PRINT_STR(bufout_trait, i16, "%hd", 10)

	DEFINE_PRINT_STR(io_trait, u16, "%hu", 10)
	DEFINE_PRINT_STR(bufout_trait, u16, "%hu", 10)

	DEFINE_PRINT_STR(io_trait, i32, "%d", 20)
	DEFINE_PRINT_STR(bufout_trait, i32, "%d", 20)

	DEFINE_PRINT_STR(io_trait, u32, "%u", 20)
	DEFINE_PRINT_STR(bufout_trait, u32, "%u", 20)

	DEFINE_PRINT_STR(io_trait, i64, "%ld", 40)
	DEFINE_PRINT_STR(bufout_trait, i64, "%ld", 40)

	DEFINE_PRINT_STR(io_trait, u64, "%lu", 40)
	DEFINE_PRINT_STR(bufout_trait, u64, "%lu", 40)

	DEFINE_PRINT_STR(io_trait, r32, "%f", 128)
	DEFINE_PRINT_STR(bufout_trait, r32, "%f", 128)

	DEFINE_PRINT_STR(io_trait, r64, "%.12f", 256)
	DEFINE_PRINT_STR(bufout_trait, r64, "%.12f", 256)

	DEFINE_PRINT_STR(io_trait, void*, "%p", 40)
	DEFINE_PRINT_STR(bufout_trait, void*, "%p", 40)

	#undef DEFINE_PRINT_STR

	//section(vprints)
	inline static usize
	vprints(io_trait *trait)
	{
		return 0;
	}

	inline static usize
	vprints(bufout_trait *trait)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vprints(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += print_str(trait, std::forward<TFirst>(first_arg));
		result += vprints(trait, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vprints(bufout_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += print_str(trait, std::forward<TFirst>(first_arg));
		result += vprints(trait, std::forward<TArgs>(args)...);
		return result;
	}


	//section(scan)
	//section(scan_bin)
	template<typename T>
	inline static usize
	scan_bin(io_trait *trait, T& value)
	{
		return trait->read(make_slice((byte*)&value, sizeof(T)));
	}

	template<typename T>
	inline static usize
	scan_bin(bufin_trait *trait, T& value)
	{
		return trait->read(make_slice((byte*)&value, sizeof(T)));
	}


	template<typename T>
	inline static usize
	scan_bin(io_trait *trait, slice<T>& values)
	{
		return trait->read(values.template convert<byte>());
	}

	template<typename T>
	inline static usize
	scan_bin(bufin_trait *trait, slice<T>& values)
	{
		return trait->read(values.template convert<byte>());
	}


	//section(vscanb)
	inline static usize
	vscanb(io_trait *trait)
	{
		return 0;
	}

	inline static usize
	vscanb(bufin_trait *trait)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vscanb(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += scan_bin(trait, std::forward<TFirst>(first_arg));
		result += vscanb(trait, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vscanb(bufin_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += scan_bin(trait, std::forward<TFirst>(first_arg));
		result += vscanb(trait, std::forward<TArgs>(args)...);
		return result;
	}


	//section(scan_str)

	//section(vscans)
	inline static usize
	vscans(io_trait *trait)
	{
		return 0;
	}

	inline static usize
	vscans(bufin_trait *trait)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vscans(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += scan_str(trait, std::forward<TFirst>(first_arg));
		result += vscans(trait, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vscans(bufin_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += scan_str(trait, std::forward<TFirst>(first_arg));
		result += vscans(trait, std::forward<TArgs>(args)...);
		return result;
	}


	//section(locks)
	API_CPPR void
	_acquire_print_lock();

	API_CPPR void
	_release_print_lock();

	API_CPPR void
	_acquire_print_err_lock();

	API_CPPR void
	_release_print_err_lock();


	//section(print)
	template<typename ... TArgs>
	inline static usize
	print(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_lock();
		result += vprints(cppr_out, std::forward<TArgs>(args)...);
		_release_print_lock();
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	print_err(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_err_lock();
		result += vprints(cppr_err, std::forward<TArgs>(args)...);
		_release_print_err_lock();
		return result;
	}


	//section(println)
	template<typename ... TArgs>
	inline static usize
	println(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_lock();
		result += vprints(cppr_out, std::forward<TArgs>(args)..., "\n");
		_release_print_lock();
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	println_err(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_err_lock();
		result += vprints(cppr_err, std::forward<TArgs>(args)..., "\n");
		_release_print_err_lock();
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	scan(TArgs&& ... args)
	{
		return vscans(buf_stdin, std::forward<TArgs>(args)...);
	}

	inline static void
	_platform_specific_to_string(const slice<byte>& data, string& str)
	{
		usize str_size = data.size;
		//check for \r
		if (data[str_size - 1] == '\r')
			--str_size;

		str._context->template realloc<byte>(str._data, str_size + 1);
		str._count = static_cast<usize>(-1);

		copy_slice(str._data, data, data.size);
		str._data[str_size] = 0;
	}

	inline static usize
	vscanln(bufin_trait *trait, string& str)
	{
		usize peek_request_size = KILOBYTES(1);
		usize parsed_peeked_size = 0;

		while (true)
		{
			slice<byte> peek_response = trait->peek(peek_request_size);
			if (!peek_response.valid())
				break;

			bool found_ln = false;
			usize it_ix = parsed_peeked_size;
			byte *it = peek_response.ptr + it_ix;
			while (it_ix < peek_response.size)
			{
				if (*it == '\n')
				{
					found_ln = true;
					break;
				}
				++it;
				++it_ix;
			}

			if (found_ln == true)
			{
				_platform_specific_to_string(peek_response.view_bytes(0, it_ix), str);
				//+1 to skip the new line itself
				trait->skip(it_ix + 1);
				return it_ix + 1;
			}

			parsed_peeked_size += peek_response.size;
			peek_request_size = parsed_peeked_size + KILOBYTES(1);
		}

		return 0;
	}

	inline static usize
	scanln(string& str)
	{
		return vscanln(buf_stdin, str);
		//usize peek_request_size = KILOBYTES(1);
		//usize parsed_peeked_size = 0;

		//usize line_limit = 0;

		//while(true)
		//{
		//	slice<byte> peek_response = buf_stdin->peek(peek_request_size);

		//	bool found_ln = false;
		//	usize it_ix = parsed_peeked_size;
		//	byte *it = peek_response.ptr + it_ix;
		//	while(it_ix < peek_response.size)
		//	{
		//		if(*it == '\n')
		//		{
		//			found_ln = true;
		//			break;
		//		}
		//		++it;
		//		++it_ix;
		//	}

		//	if(found_ln == true)
		//	{
		//		_platform_specific_to_string(peek_response.view_bytes(0, it_ix), str);
		//		//+1 to skip the new line itself
		//		buf_stdin->skip(it_ix + 1);
		//		return it_ix + 1;
		//	}

		//	parsed_peeked_size += peek_response.size;
		//	peek_request_size = parsed_peeked_size + KILOBYTES(1);
		//}

		//return 0;
	}



	//string concat
	template<typename ... TArgs>
	inline static string
	vconcat(memory_context *context, TArgs&& ... args)
	{
		memory_stream stream(context);
		usize printed_size = vprints(stream, std::forward<TArgs>(args)...);
		context->template realloc<byte>(stream._data, printed_size + 1);
		stream._data[printed_size] = 0;

		string result(std::move(stream._data), context);
		stream._size = 0;
		stream._cursor = 0;
		return result;
	}

	template<typename ... TArgs>
	inline static string
	concat(TArgs&& ... args)
	{
		return vconcat(platform->global_memory, std::forward<TArgs>(args)...);
	}
}