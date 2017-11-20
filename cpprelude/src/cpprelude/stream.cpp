#include "cpprelude/stream.h"
#include "cpprelude/memory.h"
#include <algorithm>
#include <cstdio>

namespace cpprelude
{
	//traits
	usize
	stream_trait::write(const slice<byte>& data)
	{
		return _write(_self, data);
	}

	usize
	stream_trait::read(slice<byte>& data)
	{
		return _read(_self, data);
	}

	usize
	stream_trait::read(slice<byte>&& data)
	{
		return read(data);
	}

	//memory stream
	static usize
	_memory_stream_write(void* _self, const slice<byte>& data)
	{
		memory_stream* self = reinterpret_cast<memory_stream*>(_self);

		//ensure there's always enough memory to write
		if(self->write_capacity() < data.size)
		{
			//determine how much we should realloc
			//amortized size which is 1.5 current capacity V.S. the actual size of the provided slice to write
			usize amortized_size = self->capacity() * 1.5f;
			usize actual_size = self->capacity() + data.size;
			usize realloc_size = std::max(amortized_size, actual_size);
			self->_context->template realloc<byte>(self->_data, realloc_size);
			//failure we don't write anything
			if(self->_data.size != realloc_size)
				return 0;
		}

		//success we just need to copy the data into the buffer
		copy_slice<byte>(self->_data.view(self->size()), data, data.size);
		self->_write_head += data.size;
		return data.size;
	}

	static usize
	_memory_stream_read(void* _self, slice<byte>& data)
	{
		memory_stream* self = reinterpret_cast<memory_stream*>(_self);
		//determine how much data should we read
		//available data to read in the buffer itself V.S. size of the provided slice to read into
		usize read_size = std::min(self->read_capacity(), data.size);

		//copy the data into the provided slice
		copy_slice(data, self->_data.view(self->_read_head), read_size);
		self->_read_head += read_size;
		//return how much we read from the buffer
		return read_size;
	}

	memory_stream::memory_stream()
	{
		_trait._self = this;
		_trait._write = _memory_stream_write;
		_trait._read = _memory_stream_read;
	}

	memory_stream::memory_stream(memory_stream&& other)
		:_data(std::move(other._data)),
		 _read_head(other._read_head),
		 _write_head(other._write_head),
		 _context(other._context),
		 _trait(std::move(other._trait))
	{
		_trait._self = this;
		
		other._read_head = 0;
		other._write_head = 0;
	}

	memory_stream&
	memory_stream::operator=(memory_stream&& other)
	{
		this->~memory_stream();
		_data = std::move(other._data);
		_read_head = other._read_head;
		_write_head = other._write_head;
		_context = other._context;
		_trait = std::move(other._trait);
		_trait._self = this;

		other._read_head = 0;
		other._write_head = 0;
		return *this;
	}

	memory_stream::~memory_stream()
	{
		if(_data.valid() && _context)
			_context->template free<byte>(_data);
		_read_head = 0;
		_write_head = 0;
	}

	memory_stream::operator stream_trait*()
	{
		return &_trait;
	}

	usize
	memory_stream::size() const
	{
		return _write_head;
	}

	usize
	memory_stream::capacity() const
	{
		return _data.size;
	}

	usize
	memory_stream::write_capacity() const
	{
		return _data.size - _write_head;
	}

	usize
	memory_stream::read_position() const
	{
		return _read_head;
	}

	usize
	memory_stream::read_capacity() const
	{
		return _write_head - _read_head;
	}

	bool
	memory_stream::empty() const
	{
		return _write_head == 0;
	}

	void
	memory_stream::clear()
	{
		_write_head = 0;
	}

	slice<byte>
	memory_stream::decay()
	{
		_context->template realloc<byte>(_data, _write_head);
		_read_head = 0;
		_write_head = 0;
		return std::move(_data);
	}

	slice<byte>
	memory_stream::decay_continuous()
	{
		return decay();
	}

	string
	memory_stream::string_decay()
	{
		_context->template realloc<byte>(_data, _write_head + 1);
		_data[_write_head] = 0;
		_read_head = 0;
		_write_head = 0;
		return string(std::move(_data));
	}

	//file stream
	static usize
	_file_stream_write(void* _self, const slice<byte>& data)
	{
		file_stream* self = reinterpret_cast<file_stream*>(_self);

		if(self->_cursor_position != self->_write_head)
			fseek(self->_handle, self->_write_head, SEEK_SET);

		auto result = fwrite(data.ptr, 1, data.size, self->_handle);
		self->_write_head += result;
		self->_cursor_position = self->_write_head;
		return result;
	}

	static usize
	_file_stream_read(void* _self, slice<byte>& data)
	{
		file_stream* self = reinterpret_cast<file_stream*>(_self);
		if(self->_cursor_position != self->_read_head)
			fseek(self->_handle, self->_read_head, SEEK_SET);

		auto result = fread(data.ptr, 1, data.size, self->_handle);
		self->_read_head += result;
		self->_cursor_position = self->_read_head;
		return result;
	}

	file_stream::file_stream()
	{
		_trait._self = this;
		_trait._write = _file_stream_write;
		_trait._read = _file_stream_read;
	}

	file_stream::~file_stream()
	{
		if(_handle)
			fclose(_handle);
		_read_head = 0;
		_write_head = 0;
		_cursor_position = 0;
	}

	file_stream::file_stream(file_stream&& other)
		:_handle(other._handle),
		 _read_head(other._read_head), _write_head(other._write_head),
		 mode(other.mode), name(std::move(other.name)), _trait(std::move(other._trait))
	{
		_trait._self = this;

		other._handle = nullptr;
		other._read_head = 0;
		other._write_head = 0;
	}

	file_stream&
	file_stream::operator=(file_stream&& other)
	{
		if(_handle)
			fclose(_handle);

		_handle = other._handle;
		_read_head = other._read_head;
		_write_head = other._write_head;
		mode = other.mode;
		name = std::move(other.name);

		_trait = std::move(other._trait);
		_trait._self = this;

		other._handle = nullptr;
		other._read_head = 0;
		other._write_head = 0;

		return *this;
	}

	file_stream::operator stream_trait*()
	{
		return &_trait;
	}

	bool
	file_stream::valid() const
	{
		return _handle != nullptr;
	}

	usize
	file_stream::size() const
	{
		return _write_head;
	}

	usize
	file_stream::read_position() const
	{
		return _read_head;
	}

	static void
	_open_file(file_stream& self, bool binary)
	{
		const char* cmode = nullptr;
		
		switch(self.mode)
		{
			case IO_MODE::READ:
				cmode = binary ? "rb" : "r";
				break;
			case IO_MODE::WRITE:
				cmode = binary ? "wb" : "w";
				break;
			case IO_MODE::APPEND:
				cmode = binary ? "ab" : "a";
				break;
			case IO_MODE::READ_EXTENDED:
				cmode = binary ? "rb+" : "r+";
				break;
			case IO_MODE::WRITE_EXTENDED:
				cmode = binary ? "wb+" : "w+";
				break;
			case IO_MODE::APPEND_EXTENDED:
				cmode = binary ? "ab+" : "a+";
				break;
			default:
			case IO_MODE::NONE:
				cmode = nullptr;
				break;
		}

		if(cmode)
			self._handle = std::fopen(self.name.data(), cmode);
	}

	file_stream
	open_file(const string& filename, IO_MODE openmode, bool binary)
	{
		file_stream self;
		self.mode = openmode;
		self.name = filename;

		_open_file(self, binary);
		return self;
	}

	file_stream
	open_file(string&& filename, IO_MODE openmode, bool binary)
	{
		file_stream self;
		self.mode = openmode;
		self.name = std::move(filename);

		_open_file(self, binary);
		return self;
	}
}