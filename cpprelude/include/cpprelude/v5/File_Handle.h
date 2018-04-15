#pragma once

#include "cpprelude/defines.h"

namespace cpprelude
{
	enum class OPEN_MODE2
	{
		CREATE_ONLY,		//creates the file if it doesn't exist. if it exists it fails.
		CREATE_OVERWRITE,	//creates the file if it doesn't exist. if it exists it overwrite it.
		CREATE_APPEND, 		//creates the file if it doesn't exist. if it exists it appends to it.
		OPEN_ONLY,			//opens the file if it exists. fails otherwise.
		OPEN_OVERWRITE,		//opens the file if it exist and overwrite its content. if it doesn't exist it fails.
		OPEN_APPEND			//opens the file it it exists and append to its content. if it doesn't exist it fails.
	};

	enum class IO_MODE2
	{
		READ,		//only performs reads to the file
		WRITE,		//only performs writes to the file
		READ_WRITE	//performs both reads and writes to the file
	};

	union File_Handle
	{
		void* windows_handle;
		i32 linux_handle;
	};
}