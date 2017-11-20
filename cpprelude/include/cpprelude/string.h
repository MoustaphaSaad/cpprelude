#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/platform.h"
#include <ostream>

namespace cpprelude
{
	struct rune
	{
		u32 data;

		API bool
		operator==(const rune& other) const;

		API bool
		operator!=(const rune& other) const;

		API bool
		operator<(const rune& other) const;

		API bool
		operator>(const rune& other) const;

		API bool
		operator<=(const rune& other) const;

		API bool
		operator>=(const rune& other) const;
	};

	inline static std::ostream&
	operator<<(std::ostream& out,const rune& c)
	{
		out << reinterpret_cast<const char*>(&c.data);
		return out;
	}

	inline static std::wostream&
	operator<<(std::wostream& out, const rune& c)
	{
		out << reinterpret_cast<const char*>(&c.data);
		return out;
	}

	struct rune_iterator
	{
		const byte *_ptr = nullptr;

		API rune_iterator(const byte* ptr);

		API rune_iterator&
		operator++();

		API rune_iterator
		operator++(int);

		API rune_iterator&
		operator--();

		API rune_iterator
		operator--(int);

		API bool
		operator==(const rune_iterator& other) const;

		API bool
		operator!=(const rune_iterator& other) const;

		API rune
		operator*() const;

		API operator const byte*() const;
	};

	struct string
	{
		using iterator = rune_iterator;
		using const_iterator = rune_iterator;

		slice<byte> _data;
		mutable usize _count = static_cast<usize>(-1);
		memory_context* _context = platform.global_memory;

		API string();
		API string(memory_context* context);
		API explicit string(usize size, memory_context* context = platform.global_memory);
		API string(const char* data, memory_context* context = platform.global_memory);
		API string(const slice<byte>& data, memory_context* context = platform.global_memory);
		API string(slice<byte>&& data, memory_context* context = platform.global_memory);

		API string(const string&);
		API string(const string&, memory_context* context);

		API string&
		operator=(const string&);

		API string&
		operator=(const char* data);

		API string(string&&);
		API string(string&&, memory_context* context);

		API string&
		operator=(string&&);

		API ~string();

		API usize
		count() const;

		API usize
		size() const;

		API usize
		capacity() const;

		API bool
		empty() const;

		API const byte*
		data() const;

		API byte
		operator[](usize index) const;

		API bool
		operator==(const string& str) const;

		API bool
		operator!=(const string& str) const;

		API bool
		operator<(const string& str) const;

		API bool
		operator>(const string& str) const;

		API bool
		operator<=(const string& str) const;

		API bool
		operator>=(const string& str) const;

		API const_iterator
		begin() const;

		API const_iterator
		cbegin() const;

		API const_iterator
		end() const;

		API const_iterator
		cend() const;
	};

	API cpprelude::string
	operator"" _cs(const char* str, usize str_count);

	inline static std::ostream&
	operator<<(std::ostream& out,const cpprelude::string& str)
	{
		out << reinterpret_cast<const char*>(str._data.ptr);
		return out;
	}

	inline static std::wostream&
	operator<<(std::wostream& out, const cpprelude::string& str)
	{
		out << reinterpret_cast<const char*>(str._data.ptr);
		return out;
	}
}
