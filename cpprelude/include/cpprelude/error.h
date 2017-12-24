#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/result.h"
#include "cpprelude/string.h"
#include "cpprelude/platform.h"
#include "cpprelude/io.h"

namespace cpprelude
{
	template<typename TStream>
	inline static TStream&
	operator<<(TStream& out, const PLATFORM_ERROR& err)
	{
		switch (err)
		{
		case PLATFORM_ERROR::OK:
			out << "OK";
			break;
		case PLATFORM_ERROR::GENERIC_ERROR:
			out << "GENERIC ERROR";
			break;
		case PLATFORM_ERROR::FILE_ALREADY_EXISTS:
			out << "FILE ALREADY EXISTS";
			break;
		case PLATFORM_ERROR::FILE_DOESNOT_EXIST:
			out << "FILE DOESNOT EXIST";
			break;
		default:
			out << "UNIDENTIFIED ERROR";
			break;
		}

		return out;
	}

	inline static void
	panic(const string& msg)
	{
		println_err("[panic]: ", msg);
		platform.dump_callstack();
		std::abort();
	}

	template<typename T, typename E>
	inline static T
	check(result<T, E>& res)
	{
		if(!res.has_result() || !res.ok())
			panic("failed to extract result error may have happened."_cs);

		return res;
	}

	template<typename T, typename E>
	inline static T
	check(result<T, E>&& res)
	{
		if(!res.has_result() || !res.ok())
			panic("failed to extract result error may have happened."_cs);

		return res;
	}

	template<typename T, typename E>
	inline static T
	check(result<T, E>& res, const string& msg)
	{
		if (!res.has_result() || !res.ok())
			panic(msg);

		return res;
	}

	template<typename T, typename E>
	inline static T
	check(result<T, E>&& res, const string& msg)
	{
		if (!res.has_result() || !res.ok())
			panic(msg);

		return res;
	}

	inline static void
	check(bool condition)
	{
		if(!condition)
			panic("some checked value is not true");
	}

	inline static void
	check(bool condition, const string& msg)
	{
		if(!condition)
			panic(msg);
	}
}