#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/file_defs.h"
#include "cpprelude/string.h"
#include "cpprelude/io.h"

namespace cpprelude
{
	struct file
	{
		file_handle handle;
		string name;
		io_trait _io_trait;

		API_CPPR file();

		file(const file&) = delete;

		file&
		operator=(const file&) = delete;

		API_CPPR file(file&& other);

		API_CPPR file&
		operator=(file&& other);

		API_CPPR ~file();

		inline
		operator io_trait*()
		{
			return &_io_trait;
		}

		API_CPPR bool
		valid() const;

		API_CPPR i64
		size() const;

		API_CPPR i64
		cursor() const;

		API_CPPR bool
		move(i64 offset);

		API_CPPR bool
		move_to_start();

		API_CPPR bool
		move_to_end();

		API_CPPR usize
		write(const slice<byte>& data);

		API_CPPR usize
		read(slice<byte>& data);

		API_CPPR usize
		read(slice<byte>&& data);

		API_CPPR static file
		open(const string& name,
			 IO_MODE io_mode = IO_MODE::READ_WRITE,
			 OPEN_MODE open_mode = OPEN_MODE::CREATE_OVERWRITE);

		API_CPPR static bool
		close(file& self);

		API_CPPR static bool
		close(file&& self);
	};
}