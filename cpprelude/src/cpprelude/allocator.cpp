#include "cpprelude/allocator.h"
#include <algorithm>

namespace cpprelude
{
	slice<byte>
	_arena_alloc(void* self_, usize size)
	{
		arena_t* self = (arena_t*)(self_);

		if(self->_allocation_head + size > self->_memory.size)
			return slice<byte>();

		slice<byte> result = make_slice(self->_memory.ptr + self->_allocation_head, size);
		self->_allocation_head += size;
		return result;
	}

	void
	_arena_free(void* self_, slice<byte>& data)
	{
		arena_t* self = (arena_t*)(self_);

		if(data.ptr == self->_memory.ptr + self->_allocation_head - data.size)
			self->_allocation_head -= data.size;

		data.ptr = nullptr;
		data.size = 0;
	}

	void
	_arena_realloc(void* self_, slice<byte>& data, usize size)
	{
		arena_t* self = (arena_t*)(self_);

		if(data.ptr == self->_memory.ptr + self->_allocation_head - data.size)
		{
			self->_allocation_head += size - data.size;
			data.size = size;
			return;
		}

		auto another_slice = _arena_alloc(self, size);
		copy_slice(another_slice, data, std::min(another_slice.size, data.size));
		_arena_free(self, data);
		data = another_slice;
		return;
	}

	arena_t::arena_t(usize size)
	{
		_memory = platform.virtual_alloc(nullptr, size);
		_allocation_head = 0;
		_context._self = this;
		_context._alloc = _arena_alloc;
		_context._realloc = _arena_realloc;
		_context._free = _arena_free;
	}

	arena_t::~arena_t()
	{
		if(_memory.valid())
			platform.virtual_free(_memory);
		_allocation_head = 0;
	}

	memory_context_t*
	arena_t::context()
	{
		return &_context;
	}

	void
	arena_t::free_all()
	{
		_allocation_head = 0;
	}
}
