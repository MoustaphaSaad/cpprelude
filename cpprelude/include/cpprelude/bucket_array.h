#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"
#include "cpprelude/iterator.h"

#include <cstring>

namespace cpprelude
{
	namespace details
	{
		constexpr usize
		default_size(usize element_size)
		{
			return KILOBYTES(256)/element_size > 1 ? KILOBYTES(256)/element_size : 1;
		}
	}

	template<typename T,
			 usize bucket_size = details::default_size(sizeof(T))>
	struct bucket_array
	{
		using bucket_type = T*;
		using map_type = bucket_type*;
		using iterator = bucket_array_iterator<T, bucket_size>;
		using const_iterator = const_bucket_array_iterator<T, bucket_size>;
		using data_type = T;

		map_type _map;
		memory_context *_context = platform->global_memory;
		usize _count, _bucket_count;
		iterator _begin, _end;
		iterator _cap_begin, _cap_end;

		bucket_array(memory_context* context = platform->global_memory)
			:_map(nullptr), _context(context), _count(0), _bucket_count(0)
		{ _init(); }

		bucket_array(std::initializer_list<T> list, memory_context* context = platform->global_memory)
			:_map(nullptr), _context(context), _count(0), _bucket_count(0)
		{
			_init();

			for(auto& value: list)
				insert_back(std::move(value));
		}

		bucket_array(usize count, const T& fill_value, memory_context* context = platform->global_memory)
			:_map(nullptr), _context(context), _count(0), _bucket_count(0)
		{
			_init();

			for(usize i = 0; i < count; ++i)
				insert_back(fill_value);
		}

		bucket_array(const bucket_array& other)
			:_map(nullptr), _context(other._context), _count(0), _bucket_count(0)
		{
			_init();

			for(auto& value: other)
				insert_back(value);
		}

		bucket_array(const bucket_array& other, memory_context* context)
			:_map(nullptr), _context(context), _count(0), _bucket_count(0)
		{
			_init();

			for(auto& value: other)
				insert_back(value);
		}

		bucket_array(bucket_array&& other)
			:_map(other._map),
			_context(other._context),
			_count(other._count),
			_bucket_count(other._bucket_count),
			_begin(std::move(other._begin)),
			_end(std::move(other._end)),
			_cap_begin(std::move(other._cap_begin)),
			_cap_end(std::move(other._cap_begin))
		{
			other._count = 0;
			other._bucket_count = 0;
			other._cap_begin = iterator();
			other._cap_end = iterator();
			other._begin = iterator();
			other._end = iterator();
			other._map = nullptr;
			other._context = nullptr;
		}

		bucket_array(bucket_array&& other, memory_context* context)
			:_map(other._map),
			 _context(context),
			 _count(other._count),
			 _bucket_count(other._bucket_count),
			 _begin(std::move(other._begin)),
			 _end(std::move(other._end)),
			 _cap_begin(std::move(other._cap_begin)),
			 _cap_end(std::move(other._cap_begin))
		{
			other._count = 0;
			other._bucket_count = 0;
			other._cap_begin = iterator();
			other._cap_end = iterator();
			other._begin = iterator();
			other._end = iterator();
			other._map = nullptr;
			other._context = nullptr;
		}

		~bucket_array()
		{
			_free();
		}

		bucket_array&
		operator=(const bucket_array& other)
		{
			clear();

			for(const auto& value: other)
				insert_back(value);

			return *this;
		}

		bucket_array&
		operator=(bucket_array&& other)
		{
			_free();

			_context = other._context;
			_count = other._count;
			_bucket_count = other._bucket_count;
			_cap_begin = std::move(other._cap_begin);
			_cap_end = std::move(other._cap_begin);
			_begin = std::move(other._begin);
			_end = std::move(other._end);
			_map = other._map;

			other._count = 0;
			other._bucket_count = 0;
			other._cap_begin = iterator();
			other._cap_end = iterator();
			other._begin = iterator();
			other._end = iterator();
			other._map = nullptr;
			other._context = nullptr;

			return *this;
		}

		usize
		count() const
		{
			return _count;
		}

		usize
		capacity() const
		{
			return _bucket_count * bucket_size;
		}

		void
		reserve(usize new_count)
		{
			if(new_count <= capacity())
				return;

			usize half_count = new_count/2;
			usize ceil_val = half_count % bucket_size != 0 ? 1 : 0;
			_insert_bucket_back((half_count/bucket_size) + ceil_val);
			_insert_bucket_front(half_count/bucket_size);
		}

		void
		expand_front(usize additional_count, const T& fill_value)
		{
			for(usize i = 0; i < additional_count; ++i)
				insert_front(fill_value);
		}

		void
		expand_back(usize additional_count, const T& fill_value)
		{
			for(usize i = 0; i < additional_count; ++i)
				insert_back(fill_value);
		}

		void
		shrink_front(usize shrinkage_count)
		{
			remove_front(shrinkage_count);
		}

		void
		shrink_back(usize shrinkage_count)
		{
			remove_back(shrinkage_count);
		}

		T&
		operator[](usize index)
		{
			auto it = _begin;
			while (index--)
				++it;
			return *it;
		}

		const T&
		operator[](usize index) const
		{
			auto it = _begin;
			while (index--)
				++it;
			return *it;
		}

		void
		insert_front(std::initializer_list<T> list)
		{
			auto it = list.end();
			it = std::prev(it);
			for(usize i = 0; i < list.size(); ++i)
			{
				insert_front(std::move(*it));
				it = std::prev(it);
			}
		}

		template<typename ... TArgs>
		void
		emplace_front(TArgs&& ... args)
		{
			if(_begin == _cap_begin)
				_insert_bucket_front((_bucket_count/2));

			--_begin;
			new (_begin._element_it) T(std::forward<TArgs>(args)...);
			++_count;
		}

		void
		insert_front(const T& value)
		{
			if(_begin == _cap_begin)
				_insert_bucket_front((_bucket_count/2));

			--_begin;
			*_begin = value;
			++_count;
		}

		void
		insert_front(T&& value)
		{
			if (_begin == _cap_begin)
				_insert_bucket_front((_bucket_count/2));

			--_begin;
			*_begin = std::move(value);
			++_count;
		}

		void
		insert_back(std::initializer_list<T> list)
		{
			for(auto& value: list)
				insert_back(std::move(value));
		}

		template<typename ... TArgs>
		void
		emplace_back(TArgs&& ... args)
		{
			if (_end == _cap_end)
				_insert_bucket_back((_bucket_count/2));

			new (_end._element_it) T(std::forward<TArgs>(args)...);
			++_end;
			++_count;
		}

		void
		insert_back(const T& value)
		{
			if (_end == _cap_end)
				_insert_bucket_back((_bucket_count/2));

			*_end = value;
			++_end;
			++_count;
		}

		void
		insert_back(T&& value)
		{
			if (_end == _cap_end)
				_insert_bucket_back((_bucket_count/2));

			*_end = std::move(value);
			++_end;
			++_count;
		}

		void
		remove_front(usize removal_count = 1)
		{
			while(removal_count-- && _begin != _end)
			{
				(*_begin).~T();
				++_begin;
				--_count;
			}
		}

		void
		remove_back(usize removal_count = 1)
		{
			while(removal_count-- && _begin != _end)
			{
				(*_begin).~T();
				--_end;
				--_count;
			}
		}

		void
		reset()
		{
			_free();
			_init();
		}

		void
		clear()
		{
			while(_count-- && _begin != _end)
			{
				(*_begin).~T();
				++_begin;
			}
			_count = 0;
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		const_iterator
		front() const
		{
			return _begin;
		}

		iterator
		front()
		{
			return _begin;
		}

		const_iterator
		back() const
		{
			return prev(_end);
		}

		iterator
		back()
		{
			return prev(_end);
		}

		const_iterator
		cbegin() const
		{
			return _begin;
		}

		const_iterator
		begin() const
		{
			return _begin;
		}

		iterator
		begin()
		{
			return _begin;
		}

		const_iterator
		cend() const
		{
			return _end;
		}

		const_iterator
		end() const
		{
			return _end;
		}

		iterator
		end()
		{
			return _end;
		}

		slice<T>
		decay()
		{
			return decay_continuous();
		}

		slice<T>
		decay_continuous()
		{
			slice<T> result = _context->template alloc<T>(_count);
			usize i = 0;

			for (auto& value : *this)
				new (&result[i++]) T(std::move(value));

			_free();

			return result;
		}

		void
		_init()
		{
			_bucket_count = 3;

			//allocates 3 buckets at start
			_map = _context->template alloc<bucket_type>(_bucket_count);

			//allocate the individual buckets
			_map[0] = _context->template alloc<T>(bucket_size);
			_map[1] = _context->template alloc<T>(bucket_size);
			_map[2] = _context->template alloc<T>(bucket_size);

			_init_bucket(_map[0]);
			_init_bucket(_map[1]);
			_init_bucket(_map[2]);

			_cap_begin = iterator(_map, _map[0], 0);
			_cap_end = iterator(_map + _bucket_count - 1, _map[_bucket_count - 1] + bucket_size - 1, bucket_size - 1);

			_begin = iterator(_map+1, _map[1], 0);
			_end = _begin;
		}

		void
		_insert_bucket_back(usize count = 1)
		{
			bucket_type* _new_map = _context->template alloc<bucket_type>(_bucket_count + count);

			usize i = 0;
			for(; i < _bucket_count; ++i)
			{
				_new_map[i] = _map[i];

				if(_begin._bucket_it == _map+i)
				{
					_begin = iterator(_new_map+i, _new_map[i]+_begin._index, _begin._index);
				}

				if(_end._bucket_it == _map+i)
				{
					_end = iterator(_new_map+i, _new_map[i]+_end._index, _end._index);
				}

			}

			_context->free(make_slice(_map, _bucket_count));

			_bucket_count += count;

			for (; i < _bucket_count; ++i)
			{
				_new_map[i] = _context->template alloc<T>(bucket_size);
				_init_bucket(_new_map[i]);
			}

			_map = _new_map;
			_cap_begin = iterator(_map, _map[0], 0);
			_cap_end = iterator(_map + _bucket_count - 1, _map[_bucket_count - 1] + bucket_size - 1, bucket_size - 1);
		}

		void
		_insert_bucket_front(usize count = 1)
		{
			bucket_type* _new_map = _context->template alloc<bucket_type>(_bucket_count + count);

			usize i = count;
			for (; i < _bucket_count + count; ++i)
			{
				_new_map[i] = _map[i-count];

				if (_begin._bucket_it == _map + (i - count))
				{
					_begin = iterator(_new_map + i, _new_map[i] + _begin._index, _begin._index);
				}

				if (_end._bucket_it == _map + (i - count))
				{
					_end = iterator(_new_map + i, _new_map[i] + _end._index, _end._index);
				}

			}

			_context->free(make_slice(_map, _bucket_count));

			_bucket_count += count;

			for (i = 0; i < count; ++i)
			{
				_new_map[i] = _context->template alloc<T>(bucket_size);
				_init_bucket(_new_map[i]);
			}

			_map = _new_map;
			_cap_begin = iterator(_map, _map[0], 0);
			_cap_end = iterator(_map + _bucket_count - 1, _map[_bucket_count - 1] + bucket_size - 1, bucket_size - 1);
		}

		void
		_init_bucket(bucket_type bucket)
		{
			for(usize i = 0; i < bucket_size; ++i)
				new (&bucket[i]) T();
		}

		void
		_free()
		{
			clear();

			if (_map && _context)
			{
				for (usize i = 0; i < _bucket_count; ++i)
				{
					auto other_handle = _map[i];
					_context->free(make_slice(other_handle, bucket_size));
				}

				_context->free(make_slice(_map, _bucket_count));
			}

			_map = nullptr;
			_cap_end = iterator();
			_cap_begin = iterator();
			_begin = iterator();
			_end = iterator();
			_count = 0;
			_bucket_count = 0;
		}
	};
}
