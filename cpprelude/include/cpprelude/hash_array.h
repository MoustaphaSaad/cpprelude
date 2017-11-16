#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"
#include "cpprelude/string.h"
#include "cpprelude/memory.h"
#include "cpprelude/iterator.h"

#include <new>

namespace cpprelude
{
	namespace details
	{
		//MurmurHashUnaligned2
		template<usize ptr_size>
		struct _hash_bytes
		{
			inline usize
			operator()(const void* ptr, usize len, usize seed)
			{
				static_assert(ptr_size == 0, "there's no hash function defined for this architecture pointer size");
				return 0;
			}
		};

		//4 byte pointer size architectures
		template<>
		struct _hash_bytes<4>
		{
			inline usize
			operator()(const void* ptr, usize len, usize seed)
			{
				const usize m = 0x5bd1e995;
				usize hash = seed ^ len;
				const byte* buffer = static_cast<const byte*>(ptr);

				while(len >= 4)
				{
					usize k = *reinterpret_cast<const usize*>(buffer);
					k *= m;
					k ^= k >> 24;
					k *= m;
					hash *= m;
					hash ^= k;
					buffer += 4;
					len -= 4;
				}

				if(len == 3)
				{
					hash ^= static_cast<ubyte>(buffer[2]) << 16;
					--len;
				}

				if(len == 2)
				{
					hash ^= static_cast<ubyte>(buffer[1]) << 8;
					--len;
				}

				if(len == 1)
				{
					hash ^= static_cast<ubyte>(buffer[0]);
					hash *= m;
					--len;
				}

				hash ^= hash >> 13;
				hash *= m;
				hash ^= hash >> 15;
				return hash;
			}
		};

		//8 byte pointer size architectures
		template<>
		struct _hash_bytes<8>
		{
			inline usize
			load_bytes(const byte* p, isize n) const
			{
				usize result = 0;
				--n;
				do
					result = (result << 8) + static_cast<ubyte>(p[n]);
				while(--n >= 0);
				return result;
			}

			inline usize
			shift_mix(usize v) const
			{
				return v ^ (v >> 47);
			}

			inline usize
			operator()(const void* ptr, usize len, usize seed)
			{
				static const usize mul = (static_cast<usize>(0xc6a4a793UL) << 32UL) + static_cast<usize>(0x5bd1e995UL);

				const byte* const buffer = static_cast<const byte*>(ptr);

				const i32 len_aligned = len & ~0x7;
				const byte* const end = buffer + len_aligned;

				usize hash = seed ^ (len * mul);
				for(const byte* p = buffer;
					p != end;
					p += 8)
				{
					const usize data = shift_mix((*reinterpret_cast<const usize*>(p)) * mul) * mul;
					hash ^= data;
					hash *= mul;
				}

				if((len & 0x7) != 0)
				{
					const usize data = load_bytes(end, len & 0x7);
					hash ^= data;
					hash *= mul;
				}

				hash = shift_mix(hash) * mul;
				hash = shift_mix(hash);
				return hash;
			}
		};
	}

	template<typename T>
		struct hash
		{
			inline usize
			operator()(T value) const
			{
				static_assert(sizeof(T) == 0, "there's no hash function defined for this type");
				return 0;
			}
		};

		//pointer hash
		template<>
		struct hash<void*>
		{
			inline usize
			operator()(void* ptr) const
			{
				return reinterpret_cast<usize>(ptr);
			}
		};

#define trivial_hash(TYPE)\
		template<>\
		struct hash<TYPE>\
		{\
			inline usize\
			operator()(TYPE value) const\
			{\
				return static_cast<usize>(value);\
			}\
		}

		trivial_hash(byte);

		trivial_hash(i8);
		trivial_hash(i16);
		trivial_hash(i32);
		trivial_hash(i64);

		trivial_hash(u8);
		trivial_hash(u16);
		trivial_hash(u32);
		trivial_hash(u64);

#undef trivial_hash

	API usize
	hash_bytes(const void* ptr, usize len, usize seed = 0xc70f6907UL);

	template<>
	struct hash<string>
	{
		inline usize
		operator()(const string& str) const
		{
			return hash_bytes(str.data(), str.size());
		}
	};

	template<typename T>
	struct hash<slice<T>>
	{
		inline usize
		operator()(const slice<T>& data) const
		{
			return hash_bytes(data.ptr, data.size);
		}
	};

	template<>
	struct hash<r32>
	{
		inline usize
		operator()(r32 value) const
		{
			return value != 0.0f ? hash_bytes(&value, sizeof(r32)) : 0;
		}
	};

	template<>
	struct hash<r64>
	{
		inline usize
		operator()(r64 value) const
		{
			return value != 0.0 ? hash_bytes(&value, sizeof(r64)) : 0;
		}
	};

	template<typename keyType,
			 typename valueType,
			 typename hashType = hash<keyType>>
	struct hash_array
	{
		using key_type = keyType;
		using value_type = valueType;
		using hash_type = hashType;
		using iterator = hash_array_iterator<keyType, valueType>;
		using const_iterator = const_hash_array_iterator<keyType, valueType>;
		using key_view = const_view<hash_array_key_iterator<key_type>>;
		using value_view = view<hash_array_value_iterator<value_type>,
								const_hash_array_value_iterator<value_type>>;
		using const_value_view = const_view<const_hash_array_value_iterator<value_type>>;

		dynamic_array<key_type> _keys;
		dynamic_array<value_type> _values;
		dynamic_array<u8> _flags;
		hash_type _hasher;
		usize _count;

		hash_array(memory_context* context = platform.global_memory)
			:_keys(context), _values(context), _flags(context), _count(0)
		{
			constexpr usize starting_count = 256;

			_keys.expand_back(starting_count);
			_values.expand_back(starting_count);
			_flags.expand_back(starting_count, 0);
		}

		iterator
		insert(const key_type& key)
		{
			_maintain_space_complexity();

			auto index = _find_position(key);

			//we couldn't find a position
			if(index == capacity())
				return end();

			_keys[index] = key;

			//if this cell is empty mark it full and increment _count
			if(_flags[index] == 0)
			{
				_flags[index] = 1;
				++_count;
			}

			return iterator(_keys.data() + index,
							_values.data() + index,
							_flags.data() + index,
							capacity() - index);
		}

		iterator
		insert(key_type&& key)
		{
			_maintain_space_complexity();

			auto index = _find_position(key);

			//we couldn't find a position
			if(index == capacity())
				return end();

			_keys[index] = std::move(key);

			//if this cell is empty mark it full and increment _count
			if(_flags[index] == 0)
			{
				_flags[index] = 1;
				++_count;
			}

			return iterator(_keys.data() + index,
							_values.data() + index,
							_flags.data() + index,
							capacity() - index);
		}

		iterator
		insert(const key_type& key, const value_type& value)
		{
			_maintain_space_complexity();

			auto index = _find_position(key);

			//we couldn't find a position
			if(index == capacity())
				return end();

			_keys[index] = key;
			_values[index] = value;

			//if this cell is empty mark it full and increment _count
			if(_flags[index] == 0)
			{
				_flags[index] = 1;
				++_count;
			}

			return iterator(_keys.data() + index,
							_values.data() + index,
							_flags.data() + index,
							capacity() - index);
		}

		iterator
		insert(key_type&& key, const value_type& value)
		{
			_maintain_space_complexity();

			auto index = _find_position(key);

			//we couldn't find a position
			if(index == capacity())
				return end();

			_keys[index] = std::move(key);
			_values[index] = value;

			//if this cell is empty mark it full and increment _count
			if(_flags[index] == 0)
			{
				_flags[index] = 1;
				++_count;
			}

			return iterator(_keys.data() + index,
							_values.data() + index,
							_flags.data() + index,
							capacity() - index);
		}

		iterator
		insert(const key_type& key, value_type&& value)
		{
			_maintain_space_complexity();

			auto index = _find_position(key);

			//we couldn't find a position
			if(index == capacity())
				return end();

			_keys[index] = key;
			_values[index] = std::move(value);

			//if this cell is empty mark it full and increment _count
			if(_flags[index] == 0)
			{
				_flags[index] = 1;
				++_count;
			}

			return iterator(_keys.data() + index,
							_values.data() + index,
							_flags.data() + index,
							capacity() - index);
		}

		iterator
		insert(key_type&& key, value_type&& value)
		{
			_maintain_space_complexity();

			auto index = _find_position(key);

			//we couldn't find a position
			if(index == capacity())
				return end();

			_keys[index] = std::move(key);
			_values[index] = std::move(value);

			//if this cell is empty mark it full and increment _count
			if(_flags[index] == 0)
			{
				_flags[index] = 1;
				++_count;
			}

			return iterator(_keys.data() + index,
							_values.data() + index,
							_flags.data() + index,
							capacity() - index);
		}

		iterator
		lookup(const key_type& key)
		{
			auto index = _find_position(key);

			if(_flags[index] == 0)
				return end();

			return iterator(_keys.data() + index,
							_values.data() + index,
							_flags.data() + index,
							capacity() - index);
		}

		const_iterator
		lookup(const key_type& key) const
		{
			auto index = _find_position(key);

			if(_flags[index] == 0)
				return cend();

			return const_iterator(_keys.data() + index,
								  _values.data() + index,
								  _flags.data() + index,
								  capacity() - index);
		}

		value_type&
		operator[](const key_type& key)
		{
			auto index = _find_position(key);

			//if not found then create and init one
			if(_flags[index] == 0)
			{
				_keys[index] = key;

				_flags[index] = 1;
				++_count;
			}

			return _values[index];
		}

		value_type&
		operator[](key_type&& key)
		{
			auto index = _find_position(key);

			//if not found then create and init one
			if(_flags[index] == 0)
			{
				_keys[index] = std::move(key);

				_flags[index] = 1;
				++_count;
			}

			return _values[index];
		}

		bool
		remove(const key_type& key)
		{
			auto index = _find_position(key);

			//if not found then don't remove
			if(_flags[index] == 0)
				return false;

			_flags[index] = 0;
			_keys[index].~key_type();
			_values[index].~value_type();
			--_count;
			_remove_rehash(index, _hasher(key));
			return true;
		}

		bool
		remove(const iterator& it)
		{
			//since the user has send an iterator we can deduce index
			const key_type* key_ptr = it.key_it;
			auto key_hash = _hasher(*key_ptr);
			auto index = key_ptr > _keys.data() ? key_ptr - _keys.data() :    _keys.data() - key_ptr;

			//if not found then don't remove
			if(_flags[index] == 0)
				return false;

			_flags[index] = 0;
			_keys[index].~key_type();
			_values[index].~value_type();
			--_count;
			_remove_rehash(index, key_hash);
			return true;
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		usize
		count() const
		{
			return _count;
		}

		usize
		capacity() const
		{
			return _keys.count();
		}

		void
		reserve(usize new_count)
		{
			usize cap = capacity();

			if(new_count <= cap)
				return;

			new_count = new_count - cap;

			_keys.expand_back(new_count);
			_values.expand_back(new_count);
			_flags.expand_back(new_count, 0);

			_reserve_rehash(cap);
		}

		void
		clear()
		{
			auto flag_it = _flags.begin();
			auto key_it = _keys.begin();
			auto value_it = _values.begin();

			usize count = _flags.count();
			while(count--)
			{
				if(*flag_it != 0)
				{
					*flag_it = 0;
					(*key_it).~key_type();
					(*value_it).~value_type();
				}

				flag_it = next(flag_it);
				key_it = next(key_it);
				value_it = next(value_it);
			}

			_count = 0;
		}

		iterator
		begin()
		{
			iterator result(_keys.cbegin(), _values.begin(), _flags.begin(), capacity());
			if(*result._flag_it == 0)
				++result;

			return result;
		}

		const_iterator
		begin() const
		{
			const_iterator result(_keys.cbegin(), _values.begin(), _flags.begin(), capacity());

			if(*result._flag_it == 0)
				++result;

			return result;
		}

		const_iterator
		cbegin() const
		{
			const_iterator result(_keys.cbegin(), _values.begin(), _flags.begin(), capacity());

			if(*result._flag_it == 0)
				++result;

			return result;
		}

		iterator
		end()
		{
			return iterator(_keys.cend(), _values.end(), _flags.end(), 0);
		}

		const_iterator
		end() const
		{
			return const_iterator(_keys.cend(), _values.end(), _flags.end(), 0);
		}

		const_iterator
		cend() const
		{
			return const_iterator(_keys.cend(), _values.end(), _flags.end(), 0);
		}

		key_view
		keys() const
		{
			return key_view(
				hash_array_key_iterator<key_type>(_keys.cbegin(),
										_flags.cbegin(),
										capacity()),
				hash_array_key_iterator<key_type>(_keys.cend(),
										_flags.cend(),
										0)
							);
		}

		value_view
		values()
		{
			return value_view(
				hash_array_value_iterator<value_type>(_values.begin(),
										  _flags.begin(),
										  capacity()),
				hash_array_value_iterator<value_type>(_values.end(),
										  _flags.end(),
										  0),
				const_hash_array_value_iterator<value_type>(_values.cbegin(),
												_flags.cbegin(),
												capacity()),
				const_hash_array_value_iterator<value_type>(_values.cend(),
												_flags.cend(),
												0)
							 );
		}

		const_value_view
		values() const
		{
			return const_value_view(
				const_hash_array_value_iterator<value_type>(_values.cbegin(),
												_flags.cbegin(),
												capacity()),
				const_hash_array_value_iterator<value_type>(_values.cend(),
												_flags.cend(),
												0)
									);
		}

		const_value_view
		cvalues() const
		{
			return const_value_view(
				const_hash_array_value_iterator<value_type>(_values.cbegin(),
												_flags.cbegin(),
												capacity()),
				const_hash_array_value_iterator<value_type>(_values.cend(),
												_flags.cend(),
												0)
									);
		}

		void
		_maintain_space_complexity()
		{
			if(_count > capacity() * 0.75f)
				reserve(capacity() * 2);
		}

		void
		_reserve_rehash(usize old_count)
		{
			//here we rehash everything in the old data to the new space
			for(usize i = 0;
				i < old_count;
				++i)
			{
				//if there's value here
				if(_flags[i] != 0)
				{
					//calc the new position
					usize new_index = _find_position(_keys[i]);
					//if the new index is not the same as the old then
					if(new_index != i)
					{
						//move the elements to the new index
						new (_keys.data() + new_index)  key_type(std::move(_keys[i]));
						new (_values.data() + new_index)  value_type(std::move(_values[i]));
						std::swap(_flags[i], _flags[new_index]);

					}
				}
			}
		}

		void
		_remove_rehash(usize index, usize hash)
		{
			//when remove we must linear probe and find all equal hashes and
			// relocate them one by one starting from the empty removed cell
			usize cap = capacity();
			usize i, j;
			i = (index + 1) % cap;
			j = index;

			while(i != index)
			{
				//if this is an empty cell then go out
				if(_flags[i] == 0)
					return;

				//if the hash of this key is equal to the removed hash
				//move the key of i into j
				//move the value of i into j
				//move the flag of i into j
				if(_hasher(_keys[i]) % cap == hash % cap)
				{
					new (_keys.data() + j) key_type(std::move(_keys[i]));
					new (_values.data() + j) value_type(std::move(_values[i]));
					std::swap(_flags[j], _flags[i]);
				}
				//we finished the linearly probed elements in table
				else
				{
					return;
				}

				j = i; //j is behind i by one
				++i;
				i %= cap;
			}
		}

		usize
		_find_position(const key_type& key) const
		{
			usize cap = capacity();
			usize hash_value = _hasher(key);
			usize index = hash_value % cap;
			usize it = index;

			//linear probing goes here
			while(true)
			{
				//this is an empty spot then there's no data here we return it
				if(_flags[it] == 0)
					return it;

				//this means that the position is occupied
				//check if it's the same key then we return it also
				if(_keys[it] == key)
					return it;

				//this means it position is not empty nor does it has our key
				++it;
				it %= cap;

				//if we went a full circle then we just return cap to signal
				//that we didn't find anything
				if(it == index)
					return cap;
			}

			return cap;
		}
	};

	template<typename T, typename hashType = hash<T>>
	using hash_set = hash_array<T, bool, hashType>;
}
