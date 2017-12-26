#include "cpprelude/file.h"
#include "cpprelude/platform.h"
#include "cpprelude/error.h"

namespace cpprelude
{

	usize
	_default_file_write(void* _self, const slice<byte>& data)
	{
		file* self = reinterpret_cast<file*>(_self);

		return platform->file_write(self->handle, data);
	}

	usize
	_default_file_read(void* _self, slice<byte>& data)
	{
		file* self = reinterpret_cast<file*>(_self);

		return platform->file_read(self->handle, data);
	}

	file::file()
	{
		#if defined(OS_WINDOWS)
			handle._win_handle = nullptr;
		#elif defined(OS_LINUX)
			handle._linux_handle = -1;
		#endif

		_io_trait._self = this;
		_io_trait._write = _default_file_write;
		_io_trait._read = _default_file_read;
	}

	file::file(file&& other)
		:handle(std::move(other.handle)),
		 name(std::move(other.name)),
		 _io_trait(std::move(other._io_trait))
	{
		_io_trait._self = this;

		#if defined(OS_WINDOWS)
			other.handle._win_handle = nullptr;
		#elif defined(OS_LINUX)
			other.handle._linux_handle = -1;
		#endif
	}

	file&
	file::operator=(file&& other)
	{
		handle = std::move(other.handle);
		name = std::move(other.name);
		_io_trait = std::move(other._io_trait);
		_io_trait._self = this;

		#if defined(OS_WINDOWS)
			other.handle._win_handle = nullptr;
		#elif defined(OS_LINUX)
			other.handle._linux_handle = -1;
		#endif

		return *this;
	}

	file::~file()
	{
		if(platform->file_valid(handle))
			platform->file_close(handle);
	}

	bool
	file::valid() const
	{
		return platform->file_valid(handle);
	}

	i64
	file::size() const
	{
		return platform->file_size(handle);
	}

	i64
	file::cursor() const
	{
		return platform->file_cursor(handle);
	}

	bool
	file::move(i64 offset)
	{
		return platform->file_move(handle, offset);
	}

	bool
	file::move_to_start()
	{
		return platform->file_move_to_start(handle);
	}

	bool
	file::move_to_end()
	{
		return platform->file_move_to_end(handle);
	}

	usize
	file::write(const slice<byte>& data)
	{
		return _default_file_write(this, data);
	}

	usize
	file::read(slice<byte>& data)
	{
		return _default_file_read(this, data);
	}

	usize
	file::read(slice<byte>&& data)
	{
		return _default_file_read(this, data);
	}

	file
	file::open(const string& name, IO_MODE io_mode, OPEN_MODE open_mode)
	{
		file out;
		out.name = name;
		out.handle = check(platform->file_open(out.name, io_mode, open_mode), "file failed to open"_cs);
		return out;
	}

	bool
	file::close(file& self)
	{
		return platform->file_close(self.handle);
	}

	bool
	file::close(file&& self)
	{
		return platform->file_close(self.handle);
	}
}