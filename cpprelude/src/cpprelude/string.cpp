#include "cpprelude/string.h"
#include <cstring>

namespace cpprelude
{
	bool
	rune::operator==(const rune& other) const
	{
		return data == other.data;
	}

	bool
	rune::operator!=(const rune& other) const
	{
		return data != other.data;
	}

	bool
	rune::operator<(const rune& other) const
	{
		return data < other.data;
	}

	bool
	rune::operator>(const rune& other) const
	{
		return data > other.data;
	}

	bool
	rune::operator<=(const rune& other) const
	{
		return data <= other.data;
	}

	bool
	rune::operator>=(const rune& other) const
	{
		return data >= other.data;
	}

	rune_iterator::rune_iterator(const byte* ptr)
		:_ptr(ptr)
	{}

	rune_iterator&
	rune_iterator::operator++()
	{
		if(*_ptr == 0)
			return *this;

		++_ptr;
		while(*_ptr)
		{
			if((*_ptr & 0xC0) == 0x80)
				++_ptr;
			else
				break;
		}
		return *this;
	}

	rune_iterator
	rune_iterator::operator++(int)
	{
		if(*_ptr == 0)
			return *this;
		
		auto result = *this;
		++_ptr;
		while(*_ptr)
		{
			if((*_ptr & 0xC0) == 0x80)
				++_ptr;
			else
				break;
		}
		return result;
	}

	rune_iterator&
	rune_iterator::operator--()
	{
		++_ptr;
		while(*_ptr)
		{
			if((*_ptr & 0xC0) == 0x80)
				++_ptr;
			else
				break;
		}
		return *this;
	}

	rune_iterator
	rune_iterator::operator--(int)
	{
		auto result = *this;
		++_ptr;
		while(*_ptr)
		{
			if((*_ptr & 0xC0) == 0x80)
				++_ptr;
			else
				break;
		}
		return result;
	}

	bool
	rune_iterator::operator==(const rune_iterator& other) const
	{
		return _ptr == other._ptr;
	}

	bool
	rune_iterator::operator!=(const rune_iterator& other) const
	{
		return _ptr != other._ptr;
	}

	rune
	rune_iterator::operator*() const
	{
		if (_ptr == nullptr || *_ptr == 0)
			return rune{ 0 };

		const byte* ptr = _ptr;

		byte result[sizeof(rune)] = {0};
		u8 ix = 0;
		result[ix] = *ptr;

		++ix;
		++ptr;
		while(*ptr)
		{
			if ((*ptr & 0xC0) == 0x80)
				result[ix++] = *ptr;
			else
				break;
			++ptr;
			++ix;
		}

		return *reinterpret_cast<rune*>(result);
	}

	rune_iterator::operator const byte*() const
	{
		return _ptr;
	}

	inline usize
	_count_runes(const byte* data)
	{
		usize result = 0;
		if(data)
	    	while(*data) result += (*data++ & 0xC0) != 0x80;
	    return result;
	}

	inline isize
	_strcmp(const string& a, const string& b)
	{
		auto a_it = a.begin();
		auto b_it = b.begin();

		while(a_it != a.end() && b_it != b.end())
		{
			if(*a_it != *b_it)
				break;

			++a_it;
			++b_it;
		}

		auto last_a = *a_it;
		auto last_b = *b_it;

		if(last_a > last_b)
			return 1;
		else if(last_a < last_b)
			return -1;
		else
			return 0;
	}

	string::string()
	{}

	string::string(memory_context* context)
		:_context(context)
	{}

	string::string(usize size, memory_context* context)
		:_context(context)
	{
		_data = _context->template alloc<byte>(size);
		_data[0U] = 0;
	}

	string::string(const char* data, memory_context* context)
	{
		_context = context;

		usize data_size = strlen(data);

		_data = _context->template alloc<byte>(data_size + 1);
		memcpy(_data.ptr, data, data_size);

		_data[data_size] = 0;
	}

	string::string(const slice<byte>& data, memory_context* context)
	{
		_context = context;

		_data = _context->template alloc<byte>(data.count());

		copy_slice(_data, data);
	}

	string::string(slice<byte>&& data, memory_context* context)
	{
		_context = context;
		_data = std::move(data);
	}

	string::string(const string& other)
	{
		_context = other._context;

		_data = _context->template alloc<byte>(other._data.size);

		copy_slice(_data, other._data);

		_count = other._count;
	}

	string::string(const string& other, memory_context* context)
	{
		_context = context;

		_data = _context->template alloc<byte>(other._data.size);

		copy_slice(_data, other._data);

		_count = other._count;
	}	

	string&
	string::operator=(const string& other)
	{
		_context = other._context;
		_context->template realloc<byte>(_data, other._data.size);
		copy_slice(_data, other._data);
		_count = other._count;
		return *this;
	}

	string&
	string::operator=(const char* data)
	{
		usize data_size = strlen(data);

		_context->template realloc<byte>(_data, data_size + 1);
		memcpy(_data.ptr, data, data_size);

		_data[data_size] = 0;

		_count = static_cast<usize>(-1);

		return *this;
	}

	string::string(string&& other)
	{
		_data = std::move(other._data);
		_count = other._count;
		_context = other._context;

		other._count = static_cast<usize>(-1);
		other._context = nullptr;
	}

	string::string(string&& other, memory_context* context)
	{
		_data = std::move(other._data);
		_count = other._count;
		_context = context;

		other._count = static_cast<usize>(-1);
		other._context = nullptr;
	}

	string&
	string::operator=(string&& other)
	{
		_context = other._context;
		_data = std::move(other._data);
		_count = other._count;

		other._context = nullptr;
		other._count = static_cast<usize>(-1);
		return *this;
	}

	string::~string()
	{
		if(_data.valid() && _context)
			_context->template free<byte>(_data);
		_count = static_cast<usize>(-1);
		_context = nullptr;
	}

	usize
	string::count() const
	{
		if(_count == static_cast<usize>(-1))
			_count = _count_runes(_data.ptr);
		return _count;
	}

	usize
	string::size() const
	{
		return _data.size;
	}

	usize
	string::capacity() const
	{
		return _data.size;
	}

	bool
	string::empty() const
	{
		return count() == 0;
	}

	const byte*
	string::data() const
	{
		return _data.ptr;
	}

	byte
	string::operator[](usize index) const
	{
		return _data[index];
	}

	bool
	string::operator==(const string& str) const
	{
		return _strcmp(*this, str) == 0;
	}

	bool
	string::operator!=(const string& str) const
	{
		return _strcmp(*this, str) != 0;
	}

	bool
	string::operator<(const string& str) const
	{
		return _strcmp(*this, str) < 0;
	}

	bool
	string::operator>(const string& str) const
	{
		return _strcmp(*this, str) > 0;
	}

	bool
	string::operator<=(const string& str) const
	{
		return _strcmp(*this, str) <= 0;
	}

	bool
	string::operator>=(const string& str) const
	{
		return _strcmp(*this, str) >= 0;
	}

	string::const_iterator
	string::begin() const
	{
		return const_iterator(_data.ptr);
	}

	string::const_iterator
	string::cbegin() const
	{
		return const_iterator(_data.ptr);
	}

	string::const_iterator
	string::end() const
	{
		if (_data.ptr == nullptr)
			return const_iterator(nullptr);
		return const_iterator(_data.ptr + _data.size - 1);
	}

	string::const_iterator
	string::cend() const
	{
		if (_data.ptr == nullptr)
			return const_iterator(nullptr);
		return const_iterator(_data.ptr + _data.size - 1);
	}

	string
	operator"" _cs(const char* str, usize str_count)
	{
		string result;
		//the +1 for the 0 null at the end
		result._data = make_slice<byte>((byte*)(str), str_count + 1);
		result._context = nullptr;
		result._count = _count_runes(result._data.ptr);
		return result;
	}
}