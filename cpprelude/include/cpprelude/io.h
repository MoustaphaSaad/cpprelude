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
}