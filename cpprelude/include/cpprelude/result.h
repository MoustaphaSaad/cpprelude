#pragma once

#include <cassert>

namespace cpprelude
{
	template<typename T, typename E>
	struct result
	{
		//wierd names because of macros
		enum class CPPR_RESULT_STATUS : byte {CPPR_RESULT_NONE, CPPR_RESULT_VALUE, CPPR_RESULT_ERROR};

		union
		{
			T value;
			E error;
		};
		CPPR_RESULT_STATUS _status = CPPR_RESULT_STATUS::CPPR_RESULT_NONE;

		result(const T& val)
			:value(val), _status(CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
		{}

		result(T&& val)
			:value(std::move(val)), _status(CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
		{}

		result(const E& err)
			:error(err), _status(CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
		{}

		result(E&& err)
			:error(std::move(err)), _status(CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
		{}

		result(const result&) = delete;

		result&
		operator=(const result&) = delete;

		result(result&& other)
		{
			if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
				value = std::move(other.value);
			else if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
				error = std::move(other.error);
		}

		result&
		operator=(result&& other)
		{
			if (_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
				value = std::move(other.value);
			else if (_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
				error = std::move(other.error);
			return *this;
		}

		~result()
		{
			if (_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
				value.~T();
			else if (_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
				error.~E();
			_status = CPPR_RESULT_STATUS::CPPR_RESULT_NONE;
		}

		operator T()
		{
			assert(_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE);
			_status = CPPR_RESULT_STATUS::CPPR_RESULT_NONE;
			return std::move(value);
		}

		operator E()
		{
			assert(_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR);
			_status = CPPR_RESULT_STATUS::CPPR_RESULT_NONE;
			return std::move(error);
		}

		bool
		ok() const
		{
			return _status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE;
		}

		bool
		has_result() const
		{
			return _status != CPPR_RESULT_STATUS::CPPR_RESULT_NONE;
		}
	};
}