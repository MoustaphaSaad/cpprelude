#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"

namespace cpprelude
{
	union file_handle
	{
		void* 	_win_handle;
		i32 	_linux_handle;
	};

	struct file
	{
		file_handle handle;
	};
}