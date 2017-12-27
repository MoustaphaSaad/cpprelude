#include "cpprelude/bufio.h"
#include "cpprelude/memory.h"
#include <algorithm>

namespace cpprelude
{
	slice<byte>
	_buf_reader_peek(void* _self, usize size)
	{
		buf_reader *self = reinterpret_cast<buf_reader*>(_self);

		usize available_size = self->_buffer.size() - self->_buffer.cursor();

		if(size <= available_size)
			return self->_buffer._data.view_bytes(self->_buffer.cursor(), size);

		usize size_diff = size - available_size;
		//try to read some data to match the demand
		self->_buffer.reserve(size_diff);

		usize read_size = self->_input_io->read(
			self->_buffer._data.view_bytes(self->_buffer.cursor() + available_size,
				size_diff));

		self->_buffer._size += read_size;

		if (available_size + read_size == 0)
			return slice<byte>();

		return self->_buffer._data.view_bytes(self->_buffer.cursor(), available_size + read_size);
	}

	usize
	_buf_reader_skip(void* _self, usize size)
	{
		buf_reader *self = reinterpret_cast<buf_reader*>(_self);

		usize available_size = self->_buffer.size() - self->_buffer.cursor();
		usize moving_size = std::min(available_size, size);

		//move the cursor
		self->_buffer._cursor += moving_size;

		available_size -= moving_size;

		//if we skipped the whole buffered data then we just clear the memory stream
		if(available_size == 0)
			self->_buffer.clear();

		//if the cursor is bigger than 1 megabytes and
		//the amount consumed by the cursor is bigger than the available data then we need to do something and
		//the available size is not 0
		if (self->_buffer.cursor() > MEGABYTES(1) &&
			self->_buffer.cursor() > available_size &&
			available_size != 0)
		{
			//we need some kind of memmove
			move_slice (self->_buffer._data.view_bytes(0, self->_buffer.cursor()),
						self->_buffer._data.view_bytes(self->_buffer.cursor(), available_size),
						available_size);
		}

		return moving_size;
	}

	usize
	_buf_reader_available(void* _self)
	{
		buf_reader *self = reinterpret_cast<buf_reader*>(_self);

		return self->_buffer.size() - self->_buffer.cursor();
	}

	buf_reader::buf_reader(io_trait *trait, memory_context *context)
		:_context(context),
		 _buffer(context),
		 _input_io(trait)
	{
		_bufin_trait._self = this;
		_bufin_trait._peek = _buf_reader_peek;
		_bufin_trait._skip = _buf_reader_skip;
		_bufin_trait._available = _buf_reader_available;
	}

	buf_reader::buf_reader(buf_reader&& other)
		:_context(other._context),
		 _buffer(std::move(other._buffer)),
		 _input_io(other._input_io),
		 _bufin_trait(std::move(other._bufin_trait))
	{
		_bufin_trait._self = this;

		other._input_io = nullptr;
	}

	buf_reader::buf_reader(buf_reader&& other, memory_context *context)
		:_context(context),
		 _buffer(std::move(other._buffer), _context),
		 _input_io(other._input_io),
		 _bufin_trait(std::move(other._bufin_trait))
	{
		_bufin_trait._self = this;

		other._input_io = nullptr;
	}

	buf_reader&
	buf_reader::operator=(buf_reader&& other)
	{
		_context = other._context;
		_buffer = std::move(other._buffer);
		_input_io = other._input_io;
		_bufin_trait = std::move(other._bufin_trait);

		_bufin_trait._self = this;

		other._input_io = nullptr;

		return *this;
	}

	slice<byte>
	buf_reader::peek(usize size)
	{
		return _buf_reader_peek(this, size);
	}

	usize
	buf_reader::skip(usize size)
	{
		return _buf_reader_skip(this, size);
	}

	usize
	buf_reader::available()
	{
		return _buf_reader_available(this);
	}

	usize
	buf_reader::read(slice<byte>& data)
	{
		return _bufin_trait.read(data);
	}

	usize
	buf_reader::read(slice<byte>&& data)
	{
		return _bufin_trait.read(data);
	}


	//buffered writer
	usize
	_buf_writer_write(void *_self, const slice<byte>& data)
	{
		buf_writer* self = reinterpret_cast<buf_writer*>(_self);

		usize result = 0;
		if(data.size > MEGABYTES(1))
		{
			self->flush();
			result = self->_output_io->write(data);
		}
		else
		{
			result = self->_buffer.write(data);
			if(self->_buffer.size() > MEGABYTES(1))
				self->flush();
		}

		return result;
	}

	usize
	_buf_writer_flush(void *_self)
	{
		buf_writer *self = reinterpret_cast<buf_writer*>(_self);

		usize result = 0;
		if(self->_buffer.size() > 0)
		{
			result = self->_output_io->write(self->_buffer._data.view_bytes(0, self->_buffer.size()));
			self->_buffer.clear();
		}
		return result;
	}

	usize
	_buf_writer_available(void *_self)
	{
		buf_writer *self = reinterpret_cast<buf_writer*>(_self);

		return self->_buffer.size();
	}

	buf_writer::buf_writer(io_trait *trait, memory_context *context)
		:_context(context),
		 _buffer(context),
		 _output_io(trait)
	{
		_bufout_trait._self = this;
		_bufout_trait._write = _buf_writer_write;
		_bufout_trait._flush = _buf_writer_flush;
		_bufout_trait._available = _buf_writer_available;
	}

	buf_writer::buf_writer(buf_writer&& other)
		:_context(other._context),
		 _buffer(std::move(other._buffer)),
		 _output_io(other._output_io),
		 _bufout_trait(std::move(other._bufout_trait))
	{
		_bufout_trait._self = this;

		other._output_io = nullptr;
	}

	buf_writer::buf_writer(buf_writer&& other, memory_context *context)
		:_context(context),
		 _buffer(std::move(other._buffer), context),
		 _output_io(other._output_io),
		 _bufout_trait(std::move(other._bufout_trait))
	{
		_bufout_trait._self = this;

		other._output_io = nullptr;
	}

	buf_writer::~buf_writer()
	{
		flush();
	}

	buf_writer&
	buf_writer::operator=(buf_writer&& other)
	{
		_context = other._context;
		_buffer = std::move(other._buffer);
		_output_io = other._output_io;
		_bufout_trait = std::move(other._bufout_trait);

		_bufout_trait._self = this;

		other._output_io = nullptr;
		return *this;
	}

	usize
	buf_writer::write(const slice<byte>& data)
	{
		return _buf_writer_write(this, data);
	}

	usize
	buf_writer::flush()
	{
		return _buf_writer_flush(this);
	}

	usize
	buf_writer::available()
	{
		return _buf_writer_available(this);
	}


	//buffered standard io
	bufin_trait*
	_init_buffered_stdin()
	{
		//ensure that the stdio is initialized
		_init_stdio();
		//ensure that the platform is initialized
		_init_platform();
		static buf_reader _buf_stdin(cppr_in);
		return _buf_stdin;
	}

	bufout_trait*
	_init_buffered_stdout()
	{
		//ensure that the stdio is initialized
		_init_stdio();
		//ensure that the platform is initialized
		_init_platform();
		static buf_writer _buf_stdout(cppr_out);
		return _buf_stdout;
	}

	bufout_trait*
	_init_buffered_stderr()
	{
		//ensure that the stdio is initialized
		_init_stdio();
		//ensure that the platform is initialized
		_init_platform();
		static buf_writer _buf_stderr(cppr_err);
		return _buf_stderr;
	}

	bufin_trait *buf_stdin = _init_buffered_stdin();
	bufout_trait *buf_stdout = _init_buffered_stdout();
	bufout_trait *buf_stderr = _init_buffered_stderr();
}