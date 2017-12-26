#include "cpprelude/stream.h"
#include <algorithm>

namespace cpprelude
{
	//trait functions
	static usize
	_memory_stream_write(void *_self, const slice<byte>& data)
	{
		memory_stream *self = reinterpret_cast<memory_stream*>(_self);

		self->reserve(data.size);
		copy_slice<byte>(self->_data.view(self->_cursor), data, data.size);
		self->_cursor += data.size;
		self->_size = std::max(self->_size, self->_cursor);
		return data.size;
	}

	static usize
	_memory_stream_read(void *_self, slice<byte>& data)
	{
		memory_stream *self = reinterpret_cast<memory_stream*>(_self);

		usize readable_size = std::min(self->_size - self->_cursor, data.size);
		if(readable_size == 0)
			return 0;

		copy_slice<byte>(data, self->_data.view(self->_cursor), readable_size);
		self->_cursor += readable_size;
		return readable_size;
	}

	//memory_stream
	memory_stream::memory_stream(memory_context *context)
		:_context(context)
	{
		_io_trait._self = this;
		_io_trait._write = _memory_stream_write;
		_io_trait._read = _memory_stream_read;
	}

	memory_stream::memory_stream(const slice<byte>& data, memory_context *context)
		:_context(context)
	{
		_data = _context->template alloc<byte>(data.size);
		copy_slice(_data, data);

		_size = _data.size;
		_io_trait._self = this;
		_io_trait._write = _memory_stream_write;
		_io_trait._read = _memory_stream_read;
	}

	memory_stream::memory_stream(slice<byte>&& data, memory_context *context)
		:_data(std::move(data)),
		 _context(context)
	{
		_size = _data.size;
		_io_trait._self = this;
		_io_trait._write = _memory_stream_write;
		_io_trait._read = _memory_stream_read;
	}

	memory_stream::memory_stream(memory_stream&& other)
		:_data(std::move(other._data)),
		 _cursor(other._cursor),
		 _size(other._size),
		 _context(other._context),
		 _io_trait(std::move(other._io_trait))
	{
		_io_trait._self = this;

		other._cursor = 0;
		other._size = 0;
	}

	memory_stream::memory_stream(memory_stream&& other, memory_context *context)
		:_data(std::move(other._data)),
		 _cursor(other._cursor),
		 _size(other._size),
		 _context(context),
		 _io_trait(std::move(other._io_trait))
	{
		_io_trait._self = this;

		other._cursor = 0;
		other._size = 0;
	}

	memory_stream&
	memory_stream::operator=(memory_stream&& other)
	{
		reset();

		_context = std::move(other._context);
		_data = std::move(other._data);
		_cursor = other._cursor;
		_size = other._size;
		_io_trait = std::move(other._io_trait);

		_io_trait._self = this;

		other._cursor = 0;
		other._size = 0;
		return *this;
	}

	memory_stream::~memory_stream()
	{
		reset();
	}

	usize
	memory_stream::size() const
	{
		return _size;
	}

	usize
	memory_stream::cursor() const
	{
		return _cursor;
	}

	bool
	memory_stream::move(isize offset)
	{
		auto new_cursor = _cursor + offset;
		if(new_cursor > _size)
			return false;
		
		_cursor = new_cursor;
		return true;
	}

	bool
	memory_stream::move_to_start()
	{
		_cursor = 0;
		return true;
	}

	bool
	memory_stream::move_to_end()
	{
		_cursor = _size;
		return true;
	}

	bool
	memory_stream::empty() const
	{
		return _size == 0;
	}

	void
	memory_stream::clear()
	{
		_size = 0;
		_cursor = 0;
	}

	void
	memory_stream::reset()
	{
		_size = 0;
		_cursor = 0;
		_context->template free<byte>(_data);
	}

	usize
	memory_stream::capacity() const
	{
		return _data.size;
	}

	void
	memory_stream::reserve(usize expected_size)
	{
		auto available_capacity = _data.size - _size;
		if(available_capacity >= expected_size)
			return;

		usize actual_resize_size = _data.size + expected_size - available_capacity;
		usize amortized_resize_size = _data.size * 1.5f;

		_context->template realloc<byte>(_data, std::max(actual_resize_size, amortized_resize_size));
	}

	slice<byte>
	memory_stream::decay()
	{
		_context->template realloc<byte>(_data, _size);

		_size = 0;
		_cursor = 0;
		return std::move(_data);
	}

	slice<byte>
	memory_stream::decay_continuous()
	{
		return decay();
	}


	usize
	memory_stream::write(const slice<byte>& data)
	{
		return _memory_stream_write(this, data);
	}

	usize
	memory_stream::read(slice<byte>& data)
	{
		return _memory_stream_read(this, data);
	}

	usize
	memory_stream::read(slice<byte>&& data)
	{
		return _memory_stream_read(this, data);
	}
}
