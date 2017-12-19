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
	memory_stream::memory_stream()
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
		 _context(std::move(other._context)),
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



	// //memory stream
	// static usize
	// _memory_stream_write(void* _self, const slice<byte>& data)
	// {
	// 	memory_stream* self = reinterpret_cast<memory_stream*>(_self);

	// 	//ensure there's always enough memory to write
	// 	if(self->write_capacity() < data.size)
	// 	{
	// 		//determine how much we should realloc
	// 		//amortized size which is 1.5 current capacity V.S. the actual size of the provided slice to write
	// 		usize amortized_size = self->capacity() * 1.5f;
	// 		usize actual_size = self->capacity() + data.size;
	// 		usize realloc_size = std::max(amortized_size, actual_size);
	// 		self->_context->template realloc<byte>(self->_data, realloc_size);
	// 		//failure we don't write anything
	// 		if(self->_data.size != realloc_size)
	// 			return 0;
	// 	}

	// 	//success we just need to copy the data into the buffer
	// 	copy_slice<byte>(self->_data.view(self->size()), data, data.size);
	// 	self->_write_head += data.size;
	// 	return data.size;
	// }

	// static usize
	// _memory_stream_read(void* _self, slice<byte>& data)
	// {
	// 	memory_stream* self = reinterpret_cast<memory_stream*>(_self);
	// 	//determine how much data should we read
	// 	//available data to read in the buffer itself V.S. size of the provided slice to read into
	// 	usize read_size = std::min(self->read_capacity(), data.size);

	// 	//copy the data into the provided slice
	// 	copy_slice(data, self->_data.view(self->_read_head), read_size);
	// 	self->_read_head += read_size;
	// 	//return how much we read from the buffer
	// 	return read_size;
	// }

	// memory_stream::memory_stream()
	// {
	// 	_trait._self = this;
	// 	_trait._write = _memory_stream_write;
	// 	_trait._read = _memory_stream_read;
	// }

	// memory_stream::memory_stream(memory_stream&& other)
	// 	:_data(std::move(other._data)),
	// 	 _read_head(other._read_head),
	// 	 _write_head(other._write_head),
	// 	 _context(other._context),
	// 	 _trait(std::move(other._trait))
	// {
	// 	_trait._self = this;
		
	// 	other._read_head = 0;
	// 	other._write_head = 0;
	// }

	// memory_stream&
	// memory_stream::operator=(memory_stream&& other)
	// {
	// 	this->~memory_stream();
	// 	_data = std::move(other._data);
	// 	_read_head = other._read_head;
	// 	_write_head = other._write_head;
	// 	_context = other._context;
	// 	_trait = std::move(other._trait);
	// 	_trait._self = this;

	// 	other._read_head = 0;
	// 	other._write_head = 0;
	// 	return *this;
	// }

	// memory_stream::~memory_stream()
	// {
	// 	if(_data.valid() && _context)
	// 		_context->template free<byte>(_data);
	// 	_read_head = 0;
	// 	_write_head = 0;
	// }

	// memory_stream::operator stream_trait*()
	// {
	// 	return &_trait;
	// }

	// usize
	// memory_stream::size() const
	// {
	// 	return _write_head;
	// }

	// usize
	// memory_stream::capacity() const
	// {
	// 	return _data.size;
	// }

	// usize
	// memory_stream::write_capacity() const
	// {
	// 	return _data.size - _write_head;
	// }

	// usize
	// memory_stream::read_position() const
	// {
	// 	return _read_head;
	// }

	// usize
	// memory_stream::read_capacity() const
	// {
	// 	return _write_head - _read_head;
	// }

	// bool
	// memory_stream::empty() const
	// {
	// 	return _write_head == 0;
	// }

	// void
	// memory_stream::clear()
	// {
	// 	_write_head = 0;
	// }

	// slice<byte>
	// memory_stream::decay()
	// {
	// 	_context->template realloc<byte>(_data, _write_head);
	// 	_read_head = 0;
	// 	_write_head = 0;
	// 	return std::move(_data);
	// }

	// slice<byte>
	// memory_stream::decay_continuous()
	// {
	// 	return decay();
	// }

	// string
	// memory_stream::string_decay()
	// {
	// 	_context->template realloc<byte>(_data, _write_head + 1);
	// 	_data[_write_head] = 0;
	// 	_read_head = 0;
	// 	_write_head = 0;
	// 	return string(std::move(_data));
	// }

	// //file stream
	// static usize
	// _file_stream_write(void* _self, const slice<byte>& data)
	// {
	// 	file_stream* self = reinterpret_cast<file_stream*>(_self);

	// 	if(self->_cursor_position != self->_write_head)
	// 		fseek(self->_handle, self->_write_head, SEEK_SET);

	// 	auto result = fwrite(data.ptr, 1, data.size, self->_handle);
	// 	self->_write_head += result;
	// 	self->_cursor_position = self->_write_head;
	// 	return result;
	// }

	// static usize
	// _file_stream_read(void* _self, slice<byte>& data)
	// {
	// 	file_stream* self = reinterpret_cast<file_stream*>(_self);
	// 	if(self->_cursor_position != self->_read_head)
	// 		fseek(self->_handle, self->_read_head, SEEK_SET);

	// 	auto result = fread(data.ptr, 1, data.size, self->_handle);
	// 	self->_read_head += result;
	// 	self->_cursor_position = self->_read_head;
	// 	return result;
	// }

	// file_stream::file_stream()
	// {
	// 	_trait._self = this;
	// 	_trait._write = _file_stream_write;
	// 	_trait._read = _file_stream_read;
	// }

	// file_stream::~file_stream()
	// {
	// 	if(_handle)
	// 		fclose(_handle);
	// 	_read_head = 0;
	// 	_write_head = 0;
	// 	_cursor_position = 0;
	// }

	// file_stream::file_stream(file_stream&& other)
	// 	:_handle(other._handle),
	// 	 _read_head(other._read_head), _write_head(other._write_head),
	// 	 mode(other.mode), name(std::move(other.name)), _trait(std::move(other._trait))
	// {
	// 	_trait._self = this;

	// 	other._handle = nullptr;
	// 	other._read_head = 0;
	// 	other._write_head = 0;
	// }

	// file_stream&
	// file_stream::operator=(file_stream&& other)
	// {
	// 	if(_handle)
	// 		fclose(_handle);

	// 	_handle = other._handle;
	// 	_read_head = other._read_head;
	// 	_write_head = other._write_head;
	// 	mode = other.mode;
	// 	name = std::move(other.name);

	// 	_trait = std::move(other._trait);
	// 	_trait._self = this;

	// 	other._handle = nullptr;
	// 	other._read_head = 0;
	// 	other._write_head = 0;

	// 	return *this;
	// }

	// file_stream::operator stream_trait*()
	// {
	// 	return &_trait;
	// }

	// bool
	// file_stream::valid() const
	// {
	// 	return _handle != nullptr;
	// }

	// usize
	// file_stream::size() const
	// {
	// 	return _write_head;
	// }

	// usize
	// file_stream::read_position() const
	// {
	// 	return _read_head;
	// }

	// static void
	// _open_file(file_stream& self, bool binary)
	// {
	// 	const char* cmode = nullptr;
		
	// 	switch(self.mode)
	// 	{
	// 		case IO_MODE::READ:
	// 			cmode = binary ? "rb" : "r";
	// 			break;
	// 		case IO_MODE::WRITE:
	// 			cmode = binary ? "wb" : "w";
	// 			break;
	// 		case IO_MODE::APPEND:
	// 			cmode = binary ? "ab" : "a";
	// 			break;
	// 		case IO_MODE::READ_EXTENDED:
	// 			cmode = binary ? "rb+" : "r+";
	// 			break;
	// 		case IO_MODE::WRITE_EXTENDED:
	// 			cmode = binary ? "wb+" : "w+";
	// 			break;
	// 		case IO_MODE::APPEND_EXTENDED:
	// 			cmode = binary ? "ab+" : "a+";
	// 			break;
	// 		default:
	// 		case IO_MODE::NONE:
	// 			cmode = nullptr;
	// 			break;
	// 	}

	// 	if(cmode)
	// 		self._handle = std::fopen(self.name.data(), cmode);
	// }

	// file_stream
	// open_file(const string& filename, IO_MODE openmode, bool binary)
	// {
	// 	file_stream self;
	// 	self.mode = openmode;
	// 	self.name = filename;

	// 	_open_file(self, binary);
	// 	return self;
	// }

	// file_stream
	// open_file(string&& filename, IO_MODE openmode, bool binary)
	// {
	// 	file_stream self;
	// 	self.mode = openmode;
	// 	self.name = std::move(filename);

	// 	_open_file(self, binary);
	// 	return self;
	// }
}
