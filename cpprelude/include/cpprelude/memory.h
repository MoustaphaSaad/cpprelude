#pragma once

#include "cpprelude/defines.h"
// #include "cpprelude/platform.h"

#include <cstdlib>
#include <iostream>
#include <new>
#include <cstring>

namespace cpprelude
{
	template<typename T>
	struct slice
	{
		T* ptr;
		usize size;

		slice(T* ptr_, usize size_)
			:ptr(ptr_), size(size_)
		{}

		slice()
			:ptr(nullptr), size(0)
		{}

		slice(const slice& other) = default;

		slice&
		operator=(const slice& other) = default;

		slice(slice&& other)
			:ptr(other.ptr), size(other.size)
		{
			other.ptr = nullptr;
			other.size = 0;
		}

		slice&
		operator=(slice&& other)
		{
			ptr = other.ptr;
			size = other.size;
			other.ptr = nullptr;
			other.size = 0;
			return *this;
		}

		bool
		operator==(const slice& other) const
		{
			return ptr == other.ptr && size == other.size;
		}

		bool
		operator!=(const slice& other) const
		{
			return !operator==(other);
		}

		operator T*()
		{
			return ptr;
		}

		const T&
		operator[](const usize& index) const
		{
			return ptr[index];
		}

		T&
		operator[](const usize& index)
		{
			return ptr[index];
		}

		usize
		count() const
		{
			return size / sizeof(T);
		}

		bool
		valid() const
		{
			return ptr != nullptr && size > 0;
		}

		template<typename R>
		slice<R>
		convert() const
		{
			return slice<R>(reinterpret_cast<R*>(ptr), size);
		}

		slice<T>
		view(usize start = 0, usize count = 0)
		{
			if(count == 0)
				count = (size - (start * sizeof(T))) / sizeof(T);

			return slice<T>(ptr+start, count * sizeof(T));
		}

		slice<T>
		view_bytes(usize offset = 0, usize new_size = 0)
		{
			if(new_size == 0)
				new_size = size - offset;

			return slice<T>(reinterpret_cast<T*>(reinterpret_cast<ubyte*>(ptr)+offset), new_size);
		}

		template<typename R>
		slice<R>
		view_bytes(usize offset = 0, usize new_size = 0)
		{
			if(new_size == 0)
				new_size = size - offset;

			return slice<R>(reinterpret_cast<R*>(reinterpret_cast<ubyte*>(ptr)+offset), new_size);
		}
	};

	template<typename T>
	slice<T>
	make_slice(T* ptr, usize count = 1)
	{
		return slice<T>(ptr, count*sizeof(T));
	}

	template<typename T>
	void
	copy_slice(slice<T>& dst, const slice<T>& src, usize count = 0)
	{
		if(count == 0)
			count = src.count();
		
		std::memcpy(dst.ptr, src.ptr, count * sizeof(T));
	}

	template<typename T>
	void
	copy_slice(slice<T>&& dst, const slice<T>& src, usize count = 0)
	{
		copy_slice(dst, src, count);
	}

	template<typename T>
	void
	move_slice(slice<T>& dst, slice<T>& src, usize count = 0)
	{
		if(count == 0)
			count = src.count();
		
		std::memmove(dst.ptr, src.ptr, count * sizeof(T));
	}

	template<typename T>
	void
	move_slice(slice<T>&& dst, slice<T>& src, usize count = 0)
	{
		move_slice(dst, src, count);
	}
}
