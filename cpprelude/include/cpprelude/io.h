#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	struct io_trait
	{
		using write_func = usize(*)(void*, const slice<byte>&);
		using read_func = usize(*)(void*, slice<byte>&);

		void *_self = nullptr;
		write_func _write = nullptr;
		read_func _read = nullptr;

		API_CPPR usize
		write(const slice<byte>& data);

		API_CPPR usize
		read(slice<byte>& data);

		API_CPPR usize
		read(slice<byte>&& data);
	};

	API_CPPR extern io_trait* cppr_out;
	API_CPPR extern io_trait* cppr_err;
	API_CPPR extern io_trait* cppr_in;

	//make sure std io is initialized
	API_CPPR void
	_init_stdio();
}