#include "cpprelude/fmt.h"
#include "cpprelude/platform.h"
#include "cpprelude/string.h"
#include <mutex>

namespace cpprelude
{
	static std::mutex _print_lock;

	void
	_acquire_print_lock()
	{
		_print_lock.lock();
	}

	void
	_release_print_lock()
	{
		_print_lock.unlock();
	}

	static std::mutex _print_err_lock;

	void
	_acquire_print_err_lock()
	{
		_print_err_lock.lock();
	}

	void
	_release_print_err_lock()
	{
		_print_err_lock.unlock();
	}

	void
	_platform_specific_to_string(const slice<byte>& data, string& str)
	{
		//because windows new line is \r\n
		#if defined(OS_WINDOWS)
		{
			str._context->template realloc<byte>(str._data, data.size);
			str._count = static_cast<usize>(-1);
			copy_slice(str._data, data, data.size - 1);
			str._data[data.size - 1] = 0;
		}
		//and linux new line is \n
		#elif defined(OS_LINUX)
		{
			str._context->template realloc<byte>(str._data, data.size);
			str._count = static_cast<usize>(-1);
			copy_slice(str._data, data, data.size);
			str._data[data.size] = 0;
		}
		#endif
	}
}