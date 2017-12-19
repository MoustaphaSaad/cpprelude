#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	struct io_trait
	{
		using write_func = usize(*)(void*, const slice<byte>&);
		using read_func = usize(*)(void*, slice<byte>&);

		void *_self = nullptr;
		write_func _write = nullptr;
		read_func _read = nullptr;

		API_CPPR usize
		write(const slice<byte>& data);

		API_CPPR usize
		read(slice<byte>& data);

		API_CPPR usize
		read(slice<byte>&& data);
	};

	//printing

	//binary printing
	//print a type in binary
	template<typename T>
	inline static usize
	print_bin(io_trait *trait, const T& value)
	{
		return trait->write(make_slice((byte*)(&value), sizeof(T)));
	}

	//print a slice in binary
	template<typename T>
	inline static usize
	print_bin(io_trait *trait, const slice<T> &values)
	{
		return trait->write(values.template convert<byte>());
	}

	//print absolutely nothing
	inline static usize
	vprintb(io_trait *trait)
	{
		return 0;
	}

	//print any number of args
	template<typename TFirst, typename ... TArgs>
	inline static usize
	vprintb(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += print_bin(trait, std::forward<TFirst>(first_arg));
		result += vprintb(trait, std::forward<TArgs>(args)...);
		return result;
	}


	//string printing
	//print a type in string format
	#define DEFINE_PRINT_STR(PATTERN, TYPE, BUFFER_SIZE)\
	inline static usize\
	print_str(io_trait* trait, TYPE value)\
	{\
		byte buffer[BUFFER_SIZE];\
		int written_size = std::snprintf(buffer, BUFFER_SIZE, PATTERN, value);\
		if(written_size < 0) return 0;\
		return trait->write(make_slice<byte>(buffer, written_size));\
	}

	DEFINE_PRINT_STR("%c", byte, 6)

	DEFINE_PRINT_STR("%hhd", i8, 6)
	DEFINE_PRINT_STR("%hhu", u8, 6)

	DEFINE_PRINT_STR("%hd", i16, 10)
	DEFINE_PRINT_STR("%hu", u16, 10)

	DEFINE_PRINT_STR("%d", i32, 20)
	DEFINE_PRINT_STR("%u", u32, 20)

	DEFINE_PRINT_STR("%ld", i64, 40)
	DEFINE_PRINT_STR("%lu", u64, 40)

	DEFINE_PRINT_STR("%f", r32, 128)
	DEFINE_PRINT_STR("%f", r64, 128)

	DEFINE_PRINT_STR("%p", void*, 40)

	#undef DEFINE_PRINT_STR

	//print absolutely nothing
	inline static usize
	vprints(io_trait *trait)
	{
		return 0;
	}

	//print any number or args
	template<typename TFirst, typename ... TArgs>
	inline static usize
	vprints(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += print_str(trait, std::forward<TFirst>(first_arg));
		result += vprints(trait, std::forward<TArgs>(args)...);
		return result;
	}


	//reading

	//reading binary
	//reading a type from binary
	template<typename T>
	inline static usize
	read_bin(io_trait *trait, T& value)
	{
		return trait->read(make_slice((byte*)(&value), sizeof(T)));
	}

	template<typename T>
	inline static usize
	read_bin(io_trait *trait, T&& value)
	{
		return trait->read(make_slice((byte*)(&value), sizeof(T)));
	}

	//reading a slice from binary
	template<typename T>
	inline static usize
	read_bin(io_trait *trait, slice<T>& values)
	{
		return trait->read(values.template convert<byte>());
	}

	template<typename T>
	inline static usize
	read_bin(io_trait *trait, slice<T>&& values)
	{
		return trait->read(values.template convert<byte>());
	}

	//read absolutely nothing
	inline static usize
	vreadb(io_trait *trait)
	{
		return 0;
	}

	//read any number of args
	template<typename TFirst, typename ... TArgs>
	inline static usize
	vreadb(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += read_bin(trait, std::forward<TFirst>(first_arg));
		result += vreadb(trait, std::forward<TArgs>(args)...);
		return result;
	}


	//string reading
	//read absolutely nothing
	inline static usize
	vreads(io_trait *trait)
	{
		return 0;
	}

	//read any number of args
	template<typename TFirst, typename ... TArgs>
	inline static usize
	vreads(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += read_str(trait, std::forward<TFirst>(first_arg));
		result += vreads(trait, std::forward<TArgs>(args)...);
		return result;
	}

	API_CPPR extern io_trait* cppr_out;
	API_CPPR extern io_trait* cppr_err;
	API_CPPR extern io_trait* cppr_in;

	API_CPPR void
	_acquire_print_lock();

	API_CPPR void
	_release_print_lock();

	template<typename ... TArgs>
	inline static void
	print(TArgs&& ... args)
	{
		_acquire_print_lock();
		vprints(cppr_out, std::forward<TArgs>(args)...);
		_release_print_lock();
	}

	template<typename ... TArgs>
	inline static void
	print_err(TArgs&& ... args)
	{
		_acquire_print_lock();
		vprints(cppr_err, std::forward<TArgs>(args)...);
		_release_print_lock();
	}

	template<typename ... TArgs>
	inline static void
	println(TArgs&& ... args)
	{
		_acquire_print_lock();
		vprints(cppr_out, std::forward<TArgs>(args)..., "\n");
		_release_print_lock();
	}

	template<typename ... TArgs>
	inline static void
	println_err(TArgs&& ... args)
	{
		_acquire_print_lock();
		vprints(cppr_err, std::forward<TArgs>(args)..., "\n");
		_release_print_lock();
	}
}