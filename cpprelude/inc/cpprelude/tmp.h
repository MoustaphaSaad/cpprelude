#pragma once

#include "cpprelude/defines.h"

namespace cpprelude
{
	namespace tmp
	{
		template<typename T>
		struct remove_reference
		{ typedef T type; };

		template<typename T>
		struct remove_reference<T&>
		{ typedef T type; };

		template<typename T>
		struct remove_reference<T&&>
		{ typedef T type; };

		template<typename T>
		constexpr typename remove_reference<T>::type&&
		move(T&& value)
		{
			return static_cast<typename remove_reference<T>::type&&>(value);
		}

		template<typename T>
		constexpr inline T&&
		forward(typename remove_reference<T>::type& value) noexcept
		{
			return static_cast<T&&>(value);
		}

		template<typename T>
		constexpr inline T&&
		forward(typename remove_reference<T>::type&& value) noexcept
		{
			return static_cast<T&&>(value);
		}

		template<typename T>
		void
		swap(T& a, T& b)
		{
			T temp = tmp::move(a);
			a = tmp::move(b);
			b = tmp::move(temp);
		}
	}
}