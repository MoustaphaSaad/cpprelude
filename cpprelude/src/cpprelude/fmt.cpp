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
}