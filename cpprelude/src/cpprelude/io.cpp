#include "cpprelude/io.h"

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
}