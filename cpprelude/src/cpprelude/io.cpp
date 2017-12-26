#include "cpprelude/io.h"
#include "cpprelude/error.h"
#include <mutex>

#if defined(OS_WINDOWS)
#include <Windows.h>
#elif defined(OS_LINUX)
#include <unistd.h>
#endif

namespace cpprelude
{
	usize
	io_trait::write(const slice<byte>& data)
	{
		return _write(_self, data);
	}

	usize
	io_trait::read(slice<byte>& data)
	{
		return _read(_self, data);
	}

	usize
	io_trait::read(slice<byte>&& data)
	{
		return _read(_self, data);
	}

	//std reading and writing function
	usize
	_write_std_handle(void* self, const slice<byte>& data)
	{
		file_handle* handle = reinterpret_cast<file_handle*>(self);

		return platform->file_write(*handle, data);
	}

	usize
	_read_std_handle(void* self, slice<byte>& data)
	{
		file_handle* handle = reinterpret_cast<file_handle*>(self);

		return platform->file_read(*handle, data);
	}

	usize
	_panic_write_handle(void*, const slice<byte>&)
	{
		panic("you have attempted to write into a not write-able standard handle");
		return 0;
	}

	usize
	_panic_read_handle(void*, slice<byte>&)
	{
		panic("you have attempted to read from a not read-able standard handle");
		return 0;
	}

	//i initialize stuff here because of global variable initialization order
	io_trait*
	_init_stdout()
	{
		static io_trait _stdout;
		static file_handle _stdout_handle;
		static bool _is_initialized = false;

		if(_is_initialized)
			return &_stdout;

		#if defined(OS_WINDOWS)
		{
			_stdout_handle._win_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		}
		#elif defined(OS_LINUX)
		{
			_stdout_handle._linux_handle = STDOUT_FILENO;
		}
		#endif

		_stdout._self = &_stdout_handle;
		_stdout._write = _write_std_handle;
		_stdout._read = _panic_read_handle;

		_is_initialized = true;

		return &_stdout;
	}

	io_trait*
	_init_stderr()
	{
		static io_trait _stderr;
		static file_handle _stderr_handle;
		static bool _is_initialized = false;

		if(_is_initialized)
			return &_stderr;

		#if defined(OS_WINDOWS)
		{
			_stderr_handle._win_handle = GetStdHandle(STD_ERROR_HANDLE);
		}
		#elif defined(OS_LINUX)
		{
			_stderr_handle._linux_handle = STDERR_FILENO;
		}
		#endif

		_stderr._self = &_stderr_handle;
		_stderr._write = _write_std_handle;
		_stderr._read = _panic_read_handle;

		_is_initialized = true;

		return &_stderr;
	}

	io_trait*
	_init_stdin()
	{
		static io_trait _stdin;
		static file_handle _stdin_handle;
		static bool _is_initialized = false;

		if(_is_initialized)
			return &_stdin;

		#if defined(OS_WINDOWS)
		{
			_stdin_handle._win_handle = GetStdHandle(STD_INPUT_HANDLE);
		}
		#elif defined(OS_LINUX)
		{
			_stdin_handle._linux_handle = STDIN_FILENO;
		}
		#endif

		_stdin._self = &_stdin_handle;
		_stdin._write = _panic_write_handle;
		_stdin._read = _read_std_handle;

		_is_initialized = true;

		return &_stdin;
	}

	io_trait* cppr_out = _init_stdout();
	io_trait* cppr_err = _init_stderr();
	io_trait* cppr_in  = _init_stdin();

	//make sure std io is initialized
	void
	_init_stdio()
	{
		cppr_out = _init_stdout();
		cppr_in = _init_stdin();
		cppr_err = _init_stderr();
	}
}