#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/memory.h"
#include "cpprelude/platform.h"
#include "cpprelude/io.h"
#include "cpprelude/bufio.h"

namespace cpprelude
{
	inline static usize
	_strlen(const byte *it, usize limit = -1)
	{
		usize result = 0;
		while(*it++ != 0 && result < limit)
			++result;
		return result;
	}

	struct rune
	{
		u32 data;

		API_CPPR rune();

		API_CPPR rune(u32 c);

		API_CPPR bool
		operator==(const rune& other) const;

		API_CPPR bool
		operator!=(const rune& other) const;

		API_CPPR bool
		operator<(const rune& other) const;

		API_CPPR bool
		operator>(const rune& other) const;

		API_CPPR bool
		operator<=(const rune& other) const;

		API_CPPR bool
		operator>=(const rune& other) const;
	};

	inline static usize
	print_str(io_trait *trait, const rune &c)
	{
		byte *ptr = (byte*)(&c);
		usize size = _strlen(ptr);
		return trait->write(make_slice<byte>(ptr, size));
	}

	inline static usize
	print_str(bufout_trait *trait, const rune &c)
	{
		byte *ptr = (byte*)(&c);
		usize size = _strlen(ptr);
		return trait->write(make_slice<byte>(ptr, size));
	}

	struct rune_iterator
	{
		const byte *_ptr = nullptr;

		API_CPPR rune_iterator(const byte* ptr);

		API_CPPR rune_iterator&
		operator++();

		API_CPPR rune_iterator
		operator++(int);

		API_CPPR rune_iterator&
		operator--();

		API_CPPR rune_iterator
		operator--(int);

		API_CPPR bool
		operator==(const rune_iterator& other) const;

		API_CPPR bool
		operator!=(const rune_iterator& other) const;

		API_CPPR rune
		operator*() const;

		API_CPPR operator const byte*() const;
	};

	struct string
	{
		using iterator = rune_iterator;
		using const_iterator = rune_iterator;

		slice<byte> _data;
		mutable usize _count = static_cast<usize>(-1);
		memory_context* _context = platform->global_memory;

		API_CPPR string();
		API_CPPR string(memory_context* context);
		API_CPPR explicit string(usize size, memory_context* context = platform->global_memory);
		API_CPPR string(const char* data, memory_context* context = platform->global_memory);
		API_CPPR string(const slice<byte>& data, memory_context* context = platform->global_memory);
		API_CPPR string(slice<byte>&& data, memory_context* context = platform->global_memory);

		API_CPPR string(const string&);
		API_CPPR string(const string&, memory_context* context);

		API_CPPR string&
		operator=(const string&);

		API_CPPR string&
		operator=(const char* data);

		API_CPPR string(string&&);
		API_CPPR string(string&&, memory_context* context);

		API_CPPR string&
		operator=(string&&);

		API_CPPR ~string();

		API_CPPR usize
		count() const;

		API_CPPR usize
		size() const;

		API_CPPR usize
		capacity() const;

		API_CPPR bool
		empty() const;

		API_CPPR const byte*
		data() const;

		API_CPPR byte
		operator[](usize index) const;

		API_CPPR bool
		operator==(const string& str) const;

		API_CPPR bool
		operator!=(const string& str) const;

		API_CPPR bool
		operator<(const string& str) const;

		API_CPPR bool
		operator>(const string& str) const;

		API_CPPR bool
		operator<=(const string& str) const;

		API_CPPR bool
		operator>=(const string& str) const;

		API_CPPR const_iterator
		begin() const;

		API_CPPR const_iterator
		cbegin() const;

		API_CPPR const_iterator
		end() const;

		API_CPPR const_iterator
		cend() const;
	};

	API_CPPR cpprelude::string
	operator"" _cs(const char* str, usize str_count);

	inline static usize
	print_bin(io_trait *trait, const cpprelude::string& str)
	{
		//remove the last null from the string when printing it
		byte *ptr = str._data.ptr;
		usize size = _strlen(ptr, str._data.size);

		return trait->write(make_slice<byte>(ptr, size));
	}

	inline static usize
	print_bin(bufout_trait *trait, const cpprelude::string& str)
	{
		//remove the last null from the string when printing it
		byte *ptr = str._data.ptr;
		usize size = _strlen(ptr, str._data.size);

		return trait->write(make_slice<byte>(ptr, size));
	}


	inline static usize
	print_bin(io_trait *trait, const char* str)
	{
		return trait->write(make_slice((byte*)str, _strlen(str)));
	}

	inline static usize
	print_bin(bufout_trait *trait, const char* str)
	{
		return trait->write(make_slice((byte*)str, _strlen(str)));
	}


	inline static usize
	print_str(io_trait *trait, const cpprelude::string& str)
	{
		//remove the last null from the string when printing it
		byte *ptr = str._data.ptr;
		usize size = _strlen(ptr, str._data.size);

		return trait->write(make_slice<byte>(ptr, size));
	}

	inline static usize
	print_str(bufout_trait *trait, const cpprelude::string& str)
	{
		//remove the last null from the string when printing it
		byte *ptr = str._data.ptr;
		usize size = _strlen(ptr, str._data.size);

		return trait->write(make_slice<byte>(ptr, size));
	}


	inline static usize
	print_str(io_trait *trait, const char* str)
	{
		return trait->write(make_slice((byte*)str, _strlen(str)));
	}

	inline static usize
	print_str(bufout_trait *trait, const char* str)
	{
		return trait->write(make_slice((byte*)str, _strlen(str)));
	}


	inline static usize
	scan_bin(io_trait *trait, cpprelude::string& str)
	{
		auto result = trait->read(str._data.view_bytes(0, str._data.size - 1));
		str._data[result] = 0;
		return result ;
	}

	inline static usize
	scan_bin(bufin_trait *trait, cpprelude::string& str)
	{
		auto result = trait->read(str._data.view_bytes(0, str._data.size - 1));
		str._data[result] = 0;
		return result ;
	}


	inline static usize
	scan_bin(io_trait *trait, cpprelude::string&& str)
	{
		return scan_bin(trait, str);
	}

	inline static usize
	scan_bin(bufin_trait *trait, cpprelude::string&& str)
	{
		return scan_bin(trait, str);
	}


	inline static usize
	scan_str(io_trait *trait, cpprelude::string& str)
	{
		auto result = trait->read(str._data.view_bytes(0, str._data.size - 1));
		str._data[result] = 0;
		return result;
	}

	inline static usize
	scan_str(bufin_trait *trait, cpprelude::string& str)
	{
		auto result = trait->read(str._data.view_bytes(0, str._data.size - 1));
		str._data[result] = 0;
		return result;
	}


	inline static usize
	scan_str(io_trait *trait, cpprelude::string&& str)
	{
		return scan_str(trait, str);
	}

	inline static usize
	scan_str(bufin_trait *trait, cpprelude::string&& str)
	{
		return scan_str(trait, str);
	}
}
