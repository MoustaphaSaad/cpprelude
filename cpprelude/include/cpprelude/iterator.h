#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include<iterator>
#include <ostream>

namespace cpprelude
{
	namespace details
	{
		template<typename T>
		struct single_node
		{
			single_node<T>* next;
			T data;

			single_node()
				:next(nullptr)
			{}
		};

		template<typename T>
		struct double_node
		{
			double_node<T>* prev;
			double_node<T>* next;
			T data;

			double_node()
				:next(nullptr), prev(nullptr)
			{}
		};

		template<typename key_type, typename value_type>
		struct pair_node
		{
			key_type key;
			value_type value;

			pair_node()
			{}

			pair_node(const key_type& k)
				:key(k)
			{}

			pair_node(key_type&& k)
				:key(std::move(k))
			{}

			pair_node(const key_type& k, const value_type& v)
				:key(k), value(v)
			{}

			pair_node(key_type&& k, const value_type& v)
				:key(std::move(k)), value(v)
			{}

			pair_node(const key_type& k, value_type&& v)
				:key(k), value(std::move(v))
			{}

			pair_node(key_type&& k, value_type&& v)
				:key(std::move(k)), value(std::move(v))
			{}

			bool
			operator<(const pair_node& other) const
			{
				return key < other.key;
			}

			bool
			operator>(const pair_node& other) const
			{
				return key > other.key;
			}

			bool
			operator==(const pair_node& other) const
			{
				return key == other.key;
			}

			bool
			operator != (const pair_node<key_type, value_type> & other) const
			{
				return key != other.key;
			}
		};

		template<typename T>
		struct red_black_tree_node
		{
			enum color_type: bool { RED, BLACK };
			red_black_tree_node<T> *left, *right, *parent;
			T data;
			color_type color;

			red_black_tree_node()
				:left(nullptr),
				 right(nullptr),
				 parent(nullptr),
				 color(RED)
			{}

			red_black_tree_node(const T& data_, color_type color_ = color_type::RED)
				:left(nullptr),
				 right(nullptr),
				 parent(nullptr),
				 data(data_),
				 color(color_)
			{}

			red_black_tree_node(T&& data_, color_type color_ = color_type::RED)
				:left(nullptr),
				 right(nullptr),
				 parent(nullptr),
				 data(std::move(data_)),
				 color(color_)
			{}
		};
	}

	template<typename T>
	struct sequential_iterator:std::iterator<std::random_access_iterator_tag, T, isize, T*, T&>
	{		
		using base = std::iterator<std::random_access_iterator_tag, T, isize, T*, T&>;
		T* _element;
		using data_type = T;
		
		sequential_iterator()
			:_element(nullptr)
		{}

		sequential_iterator(T* ptr)
			:_element(ptr)
		{}


		sequential_iterator<T>&
		operator++()
		{
			_element++;
			return *this;
		}

		sequential_iterator<T>
		operator++(int)
		{
			auto result = *this;
			_element++;
			return result;
		}

	
		sequential_iterator<T>&
		operator+=(typename base::difference_type offset)
		{
			_element += offset;
			return *this;	

		}

		sequential_iterator<T>
		operator+(typename base::difference_type offset)const
		{
			sequential_iterator<T> temp = *this;
			return temp += offset;
		}
	
		sequential_iterator<T>&
		operator--()
		{
			--_element;
			return *this;
		}

		sequential_iterator<T>
		operator--(int)
		{
			auto result = *this;
			--_element;
			return result;
		}

		sequential_iterator<T>&
		operator-=(typename base::difference_type offset)
		{
			return this += -offset;
		}

		sequential_iterator<T>
		operator-(typename base::difference_type offset)
		{
			sequential_iterator<T> temp = *this;
			return temp += -offset;
		}

		typename base::difference_type
		operator-(const sequential_iterator<T>& other) const
		{
			return _element - other._element;
		}

		bool
		operator==(const sequential_iterator<T>& other) const
		{
			return _element == other._element;
		}

		bool
		operator!=(const sequential_iterator<T>& other) const
		{
			return !operator==(other);
		}

		bool
		operator<(const sequential_iterator<T>& other) const
		{
			return other - *this > 0;
		}

		bool
		operator>(const sequential_iterator<T>& other) const
		{
			return other < *this ;
		}

		bool 
		operator<=(const sequential_iterator<T>& other) const
		{
			return !(other > *this);
		}

		bool
		operator>=(const sequential_iterator<T>& other) const
		{
			return !(other < *this);
		}

		const T&
		operator*() const
		{
			return *_element;
		}

		T&
		operator*()
		{
			return *_element;
		}

		operator T*()
		{
			return _element;
		}

		operator const T*() const
		{
			return _element;
		}

		T*
		operator->()
		{
			return _element;
		}

		const T*
		operator->() const
		{
			return _element;
		}

		T&
		operator[](typename base::difference_type offset)
		{
			return *(*this += offset);				
		}

	};

	template<typename T>
	struct const_forward_iterator;

	template<typename T>
	struct forward_iterator:std::iterator<std::forward_iterator_tag, T, isize, T*, T&>
	{
		using data_type = T;
		details::single_node<T>* _node;

		forward_iterator()
			:_node(nullptr)
		{}

		forward_iterator(details::single_node<T>* node)
			:_node(node)
		{}

		forward_iterator&
		operator++()
		{
			if(_node)
				_node = _node->next;
			return *this;
		}

		forward_iterator
		operator++(int)
		{
			auto result = *this;
			if(_node)
				_node = _node->next;
			return result;
		}

		bool
		operator==(const forward_iterator& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const forward_iterator& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_forward_iterator<T>& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const const_forward_iterator<T>& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return _node->data;
		}

		T&
		operator*()
		{
			return _node->data;
		}

		T*
		operator->()
		{
			return &_node->data;
		}

		const T*
		operator->() const
		{
			return &_node->data;
		}
	};

	template<typename T>
	struct const_forward_iterator:std::iterator<std::forward_iterator_tag, const T, isize,const T*, const T&>
	{
		using data_type = const T;
		const details::single_node<T>* _node;

		const_forward_iterator()
			:_node(nullptr)
		{}

		const_forward_iterator(const details::single_node<T>* node)
			:_node(node)
		{}

		const_forward_iterator(const forward_iterator<T>& other)
			:_node(other._node)
		{}

		const_forward_iterator&
		operator++()
		{
			if(_node)
				_node = _node->next;
			return *this;
		}

		const_forward_iterator
		operator++(int)
		{
			auto result = *this;
			if(_node)
				_node = _node->next;
			return result;
		}

		bool
		operator==(const forward_iterator<T>& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const forward_iterator<T>& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_forward_iterator& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const const_forward_iterator& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return _node->data;
		}

		const T*
		operator->() const
		{
			return &_node->data;
		}
	};

	template<typename T>
	struct const_bidirectional_iterator;

	template<typename T>
	struct bidirectional_iterator:std::iterator<std::bidirectional_iterator_tag, T, isize, T*, T&>
	{
		using data_type = T;
		details::double_node<T>* _node;

		bidirectional_iterator()
			:_node(nullptr)
		{}

		bidirectional_iterator(details::double_node<T>* node)
			:_node(node)
		{}

		bidirectional_iterator&
		operator++()
		{
			if(_node->next)
				_node = _node->next;

			return *this;
		}

		bidirectional_iterator
		operator++(int)
		{
			auto result = *this;

			if(_node->next)
				_node = _node->next;

			return result;
		}

		bidirectional_iterator&
		operator--()
		{
			if(_node->prev)
				_node = _node->prev;
			return *this;
		}

		bidirectional_iterator
		operator--(int)
		{
			auto result = *this;

			if(_node->prev)
				_node = _node->prev;

			return result;
		}

		bool
		operator==(const bidirectional_iterator& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const bidirectional_iterator& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_bidirectional_iterator<T>& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const const_bidirectional_iterator<T>& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return _node->data;
		}

		T&
		operator*()
		{
			return _node->data;
		}

		T*
		operator->()
		{
			return &_node->data;
		}

		const T*
		operator->() const
		{
			return &_node->data;
		}
	};

	template<typename T>
	struct const_bidirectional_iterator:std::iterator<std::bidirectional_iterator_tag, const T, isize, const T*, const T&>
	{
		using data_type = T;
		const details::double_node<T>* _node;

		const_bidirectional_iterator()
			:_node(nullptr)
		{}

		const_bidirectional_iterator(const details::double_node<T>* node)
			:_node(node)
		{}

		const_bidirectional_iterator(const bidirectional_iterator<T>& other)
			:_node(other._node)
		{}

		const_bidirectional_iterator&
		operator++()
		{
			if(_node->next)
				_node = _node->next;

			return *this;
		}

		const_bidirectional_iterator
		operator++(int)
		{
			auto result = *this;

			if(_node->next)
				_node = _node->next;

			return result;
		}

		const_bidirectional_iterator&
		operator--()
		{
			if(_node->prev)
				_node = _node->prev;
			return *this;
		}

		const_bidirectional_iterator
		operator--(int)
		{
			auto result = *this;

			if(_node->prev)
				_node = _node->prev;

			return result;
		}

		bool
		operator==(const bidirectional_iterator<T>& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const bidirectional_iterator<T>& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_bidirectional_iterator& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const const_bidirectional_iterator& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return _node->data;
		}

		const T*
		operator->() const
		{
			return &_node->data;
		}
	};

	template<typename T, usize bucket_size>
	struct const_bucket_array_iterator;

	template<typename T, usize bucket_size>
	struct bucket_array_iterator:std::iterator<std::random_access_iterator_tag, T, isize, T*, T&>
	{
		using base = std::iterator<std::random_access_iterator_tag, T, isize, T*, T&>;
		using data_type = T;
		T** _bucket_it;
		T* _element_it;
		usize _index;
		usize _bucket_index = 0;
		usize size = bucket_size;

		bucket_array_iterator()
			:_bucket_it(nullptr), _element_it(nullptr), _index(0)
		{}

		bucket_array_iterator(T** bucket_it,
							  T* element_it, usize index)
			:_bucket_it(bucket_it), _element_it(element_it), _index(index)
		{}

		bucket_array_iterator&
		operator++()
		{
			++_index;

			if (_index >= bucket_size)
			{
				_index = 0;
				++_bucket_it;
				_element_it = *_bucket_it;
			}
			else
			{
				++_element_it;
			}

			return *this;
		}

		bucket_array_iterator
		operator++(int)
		{
			auto result = *this;

			++_index;

			if (_index >= bucket_size)
			{
				_index = 0;
				++_bucket_it;
				_element_it = *_bucket_it;
			}
			else
			{
				++_element_it;
			}

			return result;
		}

		bucket_array_iterator&
		operator+=(typename base::difference_type offset)
		{
			typename base::difference_type _offset = offset + _index;
			if (_offset >= 0 && _offset < (typename base::difference_type)bucket_size)
			{
				_index += offset;
				_element_it += offset;
			}
			else
			{
				typename base::difference_type _bucket_offset = _offset > 0 ? _offset / (typename base::difference_type) bucket_size : 
					-(typename base::difference_type)((-_offset - 1) / bucket_size) - 1;
				_bucket_it += _bucket_offset;
				_index = _offset - _bucket_offset * bucket_size;
				_element_it = *_bucket_it + _index;
			}
			
			return *this;
		}

		bucket_array_iterator
		operator+(typename base::difference_type offset)const
		{
			auto temp = *this;
			return temp += offset;
		}

		bucket_array_iterator&
		operator--()
		{
			if(_index != 0)
			{
				--_element_it;
				--_index;
			}
			else
			{
				--_bucket_it;
				_index = bucket_size - 1;
				_element_it = *_bucket_it + _index;
			}

			return *this;
		}

		bucket_array_iterator
		operator--(int)
		{
			auto result = *this;

			if(_index != 0)
			{
				--_element_it;
				--_index;
			}
			else
			{
				--_bucket_it;
				_index = bucket_size - 1;
				_element_it = *_bucket_it + _index;
			}

			return result;
		}

		bucket_array_iterator&
		operator-=(typename base::difference_type offset)
		{
			return *this += -offset;
		}

		bucket_array_iterator
		operator-(typename base::difference_type offset)
		{
			auto temp = *this;
			return temp -= offset ;
		}

		typename base::difference_type
		operator-(const bucket_array_iterator& other) const
		{
			return bucket_size * (_bucket_it - other._bucket_it - 1) + _index + (bucket_size - other._index);
		}

		typename base::difference_type
		operator-(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return bucket_size * (_bucket_it - other._bucket_it - 1) + _index + (bucket_size - other._index);
		}

		bool
		operator==(const bucket_array_iterator& other) const
		{
			return _element_it == other._element_it;
		}

		bool
		operator!=(const bucket_array_iterator& other) const
		{		
			return !operator==(other);
		}

		bool
		operator==(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return _element_it == other._element_it;
		}

		bool
		operator!=(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return !operator==(other);
		}

		bool
		operator<(const bucket_array_iterator& other) const
		{
			return other - *this > 0;
		}

		bool
		operator>(const bucket_array_iterator& other) const
		{
			return other < *this;
		}

		bool
		operator<=(const bucket_array_iterator& other) const
		{
			return !(other > *this);
		}

		bool
		operator>=(const bucket_array_iterator& other) const
		{
			return !(other < *this);
		}

		bool
		operator<(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return other - *this > 0;
		}

		bool
		operator>(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return other < *this;
		}

		bool
		operator<=(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return !(other > *this);
		}

		bool
		operator>=(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return !(other < *this);
		}
		
		const T&
		operator*() const
		{
			return *_element_it;
		}

		T&
		operator*()
		{
			return *_element_it;
		}

		T*
		operator->()
		{
			return &(*_element_it);
		}

		const T*
		operator->() const
		{
			return &(*_element_it);
		}

		T&
		operator[](typename base::difference_type offset)
		{
			return *(*this += offset);
		}

	};

	template<typename T, usize bucket_size>
	struct const_bucket_array_iterator:std::iterator<std::random_access_iterator_tag, T, isize, T*, T&>
	{
		using base = std::iterator<std::random_access_iterator_tag, T, isize, T*, T&>;
		using data_type = T;
		const T** _bucket_it;
		const T* _element_it;
		usize _index;

		const_bucket_array_iterator()
			:_bucket_it(nullptr), _element_it(nullptr), _index(0)
		{}

		const_bucket_array_iterator(const T** bucket_it,
							  const T* element_it, usize index)
			:_bucket_it(bucket_it), _element_it(element_it), _index(index)
		{}

		const_bucket_array_iterator(const bucket_array_iterator<T, bucket_size>& other)
			:_bucket_it(const_cast<const T**>(other._bucket_it)),
			 _element_it(const_cast<const T*>(other._element_it)),
			 _index(other._index)
		{}

		const_bucket_array_iterator&
		operator++()
		{
			++_index;

			if (_index >= bucket_size)
			{
				_index = 0;
				++_bucket_it;
				_element_it = *_bucket_it;
			}
			else
			{
				++_element_it;
			}

			return *this;
		}

		const_bucket_array_iterator
		operator++(int)
		{
			auto result = *this;

			++_index;

			if (_index >= bucket_size)
			{
				_index = 0;
				++_bucket_it;
				_element_it = *_bucket_it;
			}
			else
			{
				++_element_it;
			}

			return result;
		}

		const_bucket_array_iterator&
		operator+=(typename base::difference_type offset)
		{
			typename base::difference_type _offset = offset + _index;
			if (_offset >= 0 && _offset < (typename base::difference_type)bucket_size)
			{
				_index += offset;
				_element_it += offset;
			}
			else
			{
				typename base::difference_type _bucket_offset = _offset > 0 ? _offset / (typename base::difference_type) bucket_size :
					-(typename base::difference_type)((-_offset - 1) / bucket_size) - 1;
				_bucket_it += _bucket_offset;
				_index = _offset - _bucket_offset * bucket_size;
				_element_it = *_bucket_it + _index;
			}

			return *this;
		}

		const_bucket_array_iterator
		operator+(typename base::difference_type offset)const
		{
			auto temp = *this;
			return temp += offset;
		}

		const_bucket_array_iterator&
		operator--()
		{
			if(_index != 0)
			{
				--_element_it;
				--_index;
			}
			else
			{
				--_bucket_it;
				_index = bucket_size - 1;
				_element_it = *_bucket_it + _index;
			}

			return *this;
		}

		const_bucket_array_iterator
		operator--(int)
		{
			auto result = *this;

			if(_index != 0)
			{
				--_element_it;
				--_index;
			}
			else
			{
				--_bucket_it;
				_index = bucket_size - 1;
				_element_it = *_bucket_it + _index;
			}

			return result;
		}

		const_bucket_array_iterator&
		operator-=(typename base::difference_type offset)
		{
			return *this += -offset;
		}

		const_bucket_array_iterator
		operator-(typename base::difference_type offset)
		{
			auto temp = *this;
			return temp -= offset ;
		}

		typename base::difference_type
		operator-(const bucket_array_iterator<T, bucket_size>& other) const
		{
			return bucket_size * (_bucket_it - other._bucket_it - 1) + _index + (bucket_size - other._index);
		}

		typename base::difference_type
		operator-(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return bucket_size * (_bucket_it - other._bucket_it - 1) + _index + (bucket_size - other._index);
		}

		bool
		operator==(const bucket_array_iterator<T, bucket_size>& other) const
		{
			return _element_it == other._element_it;
		}

		bool
		operator!=(const bucket_array_iterator<T, bucket_size>& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return _element_it == other._element_it;
		}

		bool
		operator!=(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return !operator==(other);
		}

		bool
		operator<(const bucket_array_iterator<T, bucket_size>& other) const
		{
			return other - *this > 0;
		}

		bool
		operator>(const bucket_array_iterator<T, bucket_size>& other) const
		{
			return other < *this;
		}

		bool
		operator<=(const bucket_array_iterator<T, bucket_size>& other) const
		{
			return !(other > *this);
		}

		bool
		operator>=(const bucket_array_iterator<T, bucket_size>& other) const
		{
			return !(other < *this);
		}

		bool
		operator<(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return other - *this > 0;
		}

		bool
		operator>(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return other < *this;
		}

		bool
		operator<=(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return !(other > *this);
		}

		bool
		operator>=(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return !(other < *this);
		}

		const T&
		operator*() const
		{
			return *_element_it;
		}

		const T*
		operator->() const
		{
			return &(*_element_it);
		}

		const T&
		operator[](typename base:: difference_type offset) const
		{
			return *(*this += offset);
		}
	};

	template<typename key_type, typename value_type>
	struct const_hash_array_iterator;

	template<typename key_type, typename value_type>
	struct hash_array_iterator:std::iterator<std::forward_iterator_tag, value_type, isize, value_type*, value_type&>
	{
		sequential_iterator<const key_type> key_it;
		sequential_iterator<value_type> value_it;
		sequential_iterator<u8> _flag_it;
		usize _capacity;

		hash_array_iterator()
		{_capacity = 0;}

		hash_array_iterator(const key_type* key, value_type* value, u8* flag, usize capacity)
			:key_it(key), value_it(value), _flag_it(flag), _capacity(capacity)
		{}

		hash_array_iterator&
		operator++()
		{
			++_flag_it;
			++key_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				++value_it;
				--_capacity;
			}

			return *this;
		}

		hash_array_iterator&
		operator++(int)
		{
			auto result = *this;

			++_flag_it;
			++key_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				++value_it;
				--_capacity;
			}

			return result;
		}

		bool
		operator==(const hash_array_iterator& other) const
		{
			return _flag_it 	== other._flag_it &&
				   key_it 		== other.key_it   &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const hash_array_iterator& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_hash_array_iterator<key_type, value_type>& other) const
		{
			return _flag_it 	== other._flag_it  &&
				   key_it 		== other.key_it   &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const const_hash_array_iterator<key_type, value_type>& other) const
		{
			return !operator==(other);
		}

		const key_type&
		operator*() const
		{
			return *key_it;
		}

		const key_type&
		key() const
		{
			return *key_it;
		}

		value_type&
		value()
		{
			return *value_it;
		}

		const value_type&
		value() const
		{
			return *value_it;
		}

		value_type*
		operator->()
		{
			return value_it;
		}

		const value_type*
		operator->() const
		{
			return value_it;
		}
	};

	template<typename key_type, typename value_type>
	struct const_hash_array_iterator:std::iterator<std::forward_iterator_tag, const value_type, isize, const value_type*, const value_type&>
	{
		sequential_iterator<const key_type> key_it;
		sequential_iterator<const value_type> value_it;
		sequential_iterator<const u8> _flag_it;
		usize _capacity;

		const_hash_array_iterator()
		{_capacity = 0;}

		const_hash_array_iterator(const key_type* key, const value_type* value, const u8* flag, usize capacity)
			:key_it(key), value_it(value), _flag_it(flag), _capacity(capacity)
		{}

		const_hash_array_iterator(const hash_array_iterator<key_type, value_type>& other)
			:key_it(other.key_it),
			 value_it(other.value_it),
			 _flag_it(other._flag_it),
			 _capacity(other._capacity)
		{}

		const_hash_array_iterator&
		operator++()
		{
			++_flag_it;
			++key_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				++value_it;
				--_capacity;
			}

			return *this;
		}

		const_hash_array_iterator&
		operator++(int)
		{
			auto result = *this;

			++_flag_it;
			++key_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				++value_it;
				--_capacity;
			}

			return result;
		}

		bool
		operator==(const hash_array_iterator<key_type, value_type>& other) const
		{
			return _flag_it 	== other._flag_it &&
				   key_it 		== other.key_it   &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const hash_array_iterator<key_type, value_type>& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_hash_array_iterator& other) const
		{
			return _flag_it 	== other._flag_it &&
				   key_it 		== other.key_it   &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const const_hash_array_iterator& other) const
		{
			return !operator==(other);
		}

		const key_type&
		operator*() const
		{
			return *key_it;
		}

		const key_type&
		key() const
		{
			return *key_it;
		}

		const value_type&
		value() const
		{
			return *value_it;
		}

		const value_type*
		operator->() const
		{
			return value_it;
		}
	};

	template<typename key_type>
	struct hash_array_key_iterator:std::iterator<std::forward_iterator_tag, key_type, isize, key_type*, key_type&>
	{
		sequential_iterator<const key_type> key_it;
		sequential_iterator<const u8> _flag_it;
		usize _capacity;

		hash_array_key_iterator()
		{_capacity = 0;}

		hash_array_key_iterator(const key_type* key,const u8* flag, usize capacity)
			:key_it(key), _flag_it(flag), _capacity(capacity)
		{}

		hash_array_key_iterator&
		operator++()
		{
			++_flag_it;
			++key_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				--_capacity;
			}

			return *this;
		}

		hash_array_key_iterator&
		operator++(int)
		{
			auto result = *this;

			++_flag_it;
			++key_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				--_capacity;
			}

			return result;
		}

		bool
		operator==(const hash_array_key_iterator& other) const
		{
			return _flag_it 	== other._flag_it &&
				   key_it 		== other.key_it   &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const hash_array_key_iterator& other) const
		{
			return !operator==(other);
		}

		const key_type&
		operator*() const
		{
			return *key_it;
		}

		const key_type&
		key() const
		{
			return *key_it;
		}

		const key_type*
		operator->() const
		{
			return key_it;
		}
	};

	template<typename value_type>
	struct hash_array_value_iterator:std::iterator<std::forward_iterator_tag, value_type, isize, value_type*, value_type&>
	{
		sequential_iterator<value_type> value_it;
		sequential_iterator<u8> _flag_it;
		usize _capacity;

		hash_array_value_iterator()
		{_capacity = 0;}

		hash_array_value_iterator(value_type* value, u8* flag, usize capacity)
			:value_it(value), _flag_it(flag), _capacity(capacity)
		{}

		hash_array_value_iterator&
		operator++()
		{
			++_flag_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++value_it;
				--_capacity;
			}

			return *this;
		}

		hash_array_value_iterator&
		operator++(int)
		{
			auto result = *this;

			++_flag_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++value_it;
				--_capacity;
			}

			return result;
		}

		bool
		operator==(const hash_array_value_iterator& other) const
		{
			return _flag_it 	== other._flag_it &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const hash_array_value_iterator& other) const
		{
			return !operator==(other);
		}
		/*
		bool
		operator==(const const_hash_array_value_iterator<value_type>& other) const
		{
			return _flag_it 	== other._flag_it &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const const_hash_array_value_iterator<value_type>& other) const
		{
			return !operator==(other);
		}*/

		value_type&
		operator*()
		{
			return *value_it;
		}

		const value_type&
		operator*() const
		{
			return *value_it;
		}

		value_type&
		value()
		{
			return *value_it;
		}

		const value_type&
		value() const
		{
			return *value_it;
		}

		value_type*
		operator->()
		{
			return value_it;
		}

		const value_type*
		operator->() const
		{
			return value_it;
		}
	};

	template<typename value_type>
	struct const_hash_array_value_iterator:std::iterator<std::forward_iterator_tag, value_type, isize, value_type*, value_type&>
	{
		sequential_iterator<const value_type> value_it;
		sequential_iterator<const u8> _flag_it;
		usize _capacity;

		const_hash_array_value_iterator()
		{_capacity = 0;}

		const_hash_array_value_iterator(const value_type* value, const u8* flag, usize capacity)
			:value_it(value), _flag_it(flag), _capacity(capacity)
		{}

		const_hash_array_value_iterator&
		operator++()
		{
			++_flag_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++value_it;
				--_capacity;
			}

			return *this;
		}

		const_hash_array_value_iterator&
		operator++(int)
		{
			auto result = *this;

			++_flag_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++value_it;
				--_capacity;
			}

			return result;
		}
		
		/*
		bool
		operator==(const hash_array_value_iterator<R>& other) const
		{
			return _flag_it 	== other._flag_it &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const hash_array_value_iterator<value_type>& other) const
		{
			return !operator==(other);
		}
		*/
		bool
		operator==(const const_hash_array_value_iterator<value_type>& other) const
		{
			return _flag_it 	== other._flag_it &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const const_hash_array_value_iterator<value_type>& other) const
		{
			return !operator==(other);
		}

		const value_type&
		operator*() const
		{
			return *value_it;
		}

		const value_type&
		value() const
		{
			return *value_it;
		}

		const value_type*
		operator->() const
		{
			return value_it;
		}
	};

   	template<typename iterator, typename const_iterator>
	struct view
	{
		iterator _begin_it;
		iterator _end_it;
		const_iterator _cbegin_it;
		const_iterator _cend_it;

		view(const iterator& begin_it, const iterator& end_it,
			 const const_iterator& cbegin_it, const const_iterator& cend_it)
			:_begin_it(begin_it), _end_it(end_it),
			 _cbegin_it(cbegin_it), _cend_it(cend_it)
		{}

		view()
		{}

		iterator
		begin()
		{
			auto result = _begin_it;
			if(*result._flag_it == 0)
				++result;
			return result;
		}

		const_iterator
		begin() const
		{
			auto result = _cbegin_it;
			if(*result._flag_it == 0)
				++result;
			return result;
		}

		const_iterator
		cbegin() const
		{
			auto result = _cbegin_it;
			if(*result._flag_it == 0)
				++result;
			return result;
		}

		iterator
		end()
		{
			return _end_it;
		}

		const_iterator
		end() const
		{
			return _cend_it;
		}

		const_iterator
		cend() const
		{
			return _cend_it;
		}
	};

	template<typename const_iterator>
	struct const_view
	{
		const_iterator _cbegin_it;
		const_iterator _cend_it;

		const_view(const const_iterator& cbegin_it, const const_iterator&  cend_it)
			:_cbegin_it(cbegin_it), _cend_it(cend_it)
		{}

		const_view()
		{}

		const_iterator
		begin() const
		{
			auto result = _cbegin_it;
			if(*result._flag_it == 0)
				++result;
			return result;
		}

		const_iterator
		cbegin() const
		{
			auto result = _cbegin_it;
			if(*result._flag_it == 0)
				++result;
			return result;
		}

		const_iterator
		end() const
		{
			return _cend_it;
		}

		const_iterator
		cend() const
		{
			return _cend_it;
		}
	};

	template<typename T>
	struct red_black_tree_iterator:std::iterator<std::bidirectional_iterator_tag, T, isize, T*, T&>
	{
		using data_type = const T;
		using node_type = const details::red_black_tree_node<T>;

		node_type* node;

		red_black_tree_iterator()
			:node(nullptr)
		{}

		red_black_tree_iterator(node_type* node_)
			:node(node_)
		{}

		red_black_tree_iterator&
		operator++()
		{
			node = _get_successor(node);
			return *this;
		}

		red_black_tree_iterator
		operator++(int)
		{
			node_type* result = node;
			node = _get_successor(node);
			return red_black_tree_iterator(result);
		}

		red_black_tree_iterator&
		operator--()
		{
			node = _get_predecessor(node);
			return *this;
		}

		red_black_tree_iterator
		operator--(int)
		{
			node_type* result = node;
			node = _get_predecessor(node);
			return red_black_tree_iterator(result);
		}

		bool
		operator==(const red_black_tree_iterator& other) const
		{
			return node == other.node;
		}

		bool
		operator!=(const red_black_tree_iterator& other) const
		{
			return node != other.node;
		}

		void
		move_up() const
		{
			node = node->parent;
		}

		void
		move_left() const
		{
			node = node->left;
		}

		void
		move_right() const
		{
			node = node->right;
		}

		data_type*
		operator->() const
		{
			return &node->data;
		}

		data_type&
		operator*() const
		{
			return node->data;
		}

		//the combination of the next two functions allow this iterator to iterate on all tree nodes starting from the most left smallest key in the tree

		//this function traverses the right branch of a tree
		//gets the next node in tree which the next larger key
		node_type*
		_get_successor(node_type* node)
		{
			if (node == nullptr) return node;
			//get the right node
			node_type* successor = node->right;

			//if the right node exist then go all the way to its left branch
			if (successor != nullptr)
			{
				while (successor->left != nullptr)
					successor = successor->left;
				return successor;
			}

			//if there's no right node then go up all the way along the right branch
			node_type* temp = node;
			successor = temp->parent;
			while (successor != nullptr && temp == successor->right)
			{
				temp = successor;
				successor = successor->parent;
			}
			return successor;
		}

		//this function traverses the left branch of a tree
		//same as above but gets the next smaller key
		node_type*
		_get_predecessor(node_type* node)
		{
			if (node == nullptr) return node;
			//check the left node
			node_type* predecessor = node->left;

			//go all the war through the right branch
			if (predecessor != nullptr)
			{
				while (predecessor->right != nullptr)
					predecessor = predecessor->right;
				return predecessor;
			}

			node_type* temp = node;
			predecessor = temp->parent;
			while (predecessor != nullptr && temp == predecessor->left)
			{
				temp = predecessor;
				predecessor = predecessor->parent;
			}
			return predecessor;
		}
	};

	template<typename KeyType, typename ValueType>
	struct red_black_tree_const_pair_iterator;

	template<typename KeyType, typename ValueType>
	struct red_black_tree_pair_iterator: public red_black_tree_iterator<details::pair_node<KeyType, ValueType>>
	{
		using key_type = KeyType;
		using value_type = ValueType;
		using data_type = const details::pair_node<KeyType, ValueType>;
		using node_type = const details::red_black_tree_node<details::pair_node<KeyType, ValueType>>;
		using _implementation = red_black_tree_iterator<details::pair_node<KeyType, ValueType>>;

		red_black_tree_pair_iterator()
			:_implementation()
		{}

		red_black_tree_pair_iterator(node_type* node_)
			:_implementation(node_)
		{}

		red_black_tree_pair_iterator(const red_black_tree_iterator<details::pair_node<KeyType, ValueType>>& it)
			:_implementation(it.node)
		{}

		red_black_tree_pair_iterator&
		operator++()
		{
			this->node = _implementation::_get_successor(this->node);
			return *this;
		}

		red_black_tree_pair_iterator
		operator++(int)
		{
			node_type* result = this->node;
			this->node = _implementation::_get_successor(this->node);
			return red_black_tree_pair_iterator(result);
		}

		red_black_tree_pair_iterator&
		operator--()
		{
			this->node = _implementation::_get_predecessor(this->node);
			return *this;
		}

		red_black_tree_pair_iterator
		operator--(int)
		{
			node_type* result = this->node;
			this->node = _implementation::_get_predecessor(this->node);
			return red_black_tree_pair_iterator(result);
		}

		bool
		operator==(const red_black_tree_pair_iterator& other) const
		{
			return this->node == other.node;
		}

		bool
		operator!=(const red_black_tree_pair_iterator& other) const
		{
			return this->node != other.node;
		}

		bool
		operator==(const red_black_tree_const_pair_iterator<key_type, value_type>& other) const
		{
			return this->node == other.node;
		}

		bool
		operator!=(const red_black_tree_const_pair_iterator<key_type, value_type>& other) const
		{
			return this->node != other.node;
		}

		using _implementation::move_up;
		using _implementation::move_left;
		using _implementation::move_right;

		value_type*
		operator->()
		{
			return const_cast<value_type*>(&this->node->data.value);
		}

		const value_type*
		operator->() const
		{
			return &this->node->data.value;
		}

		const key_type&
		operator*() const
		{
			return this->node->data.key;
		}

		const key_type&
		key() const
		{
			return this->node->data.key;
		}

		value_type&
		value()
		{
			return const_cast<value_type&>(this->node->data.value);
		}

		const value_type&
		value() const
		{
			return this->node->data.value;
		}
	};

	template<typename KeyType, typename ValueType>
	struct red_black_tree_const_pair_iterator: public red_black_tree_iterator<details::pair_node<KeyType, ValueType>>
	{
		using key_type = KeyType;
		using value_type = ValueType;
		using data_type = const details::pair_node<KeyType, ValueType>;
		using node_type = const details::red_black_tree_node<details::pair_node<KeyType, ValueType>>;
		using _implementation = red_black_tree_iterator<details::pair_node<KeyType, ValueType>>;

		red_black_tree_const_pair_iterator()
			:_implementation()
		{}

		red_black_tree_const_pair_iterator(node_type* node_)
			:_implementation(node_)
		{}

		red_black_tree_const_pair_iterator(const red_black_tree_iterator<details::pair_node<KeyType, ValueType>>& it)
			:_implementation(it.node)
		{}

		red_black_tree_const_pair_iterator&
		operator++()
		{
			this->node = _implementation::_get_successor(this->node);
			return *this;
		}

		red_black_tree_const_pair_iterator
		operator++(int)
		{
			node_type* result = this->node;
			this->node = _implementation::_get_successor(this->node);
			return red_black_tree_const_pair_iterator(result);
		}

		red_black_tree_const_pair_iterator&
		operator--()
		{
			this->node = _implementation::_get_predecessor(this->node);
			return *this;
		}

		red_black_tree_const_pair_iterator
		operator--(int)
		{
			node_type* result = this->node;
			this->node = _implementation::_get_predecessor(this->node);
			return red_black_tree_const_pair_iterator(result);
		}

		bool
		operator==(const red_black_tree_const_pair_iterator& other) const
		{
			return this->node == other.node;
		}

		bool
		operator!=(const red_black_tree_const_pair_iterator& other) const
		{
			return this->node != other.node;
		}

		using _implementation::move_up;
		using _implementation::move_left;
		using _implementation::move_right;

		const value_type*
		operator->() const
		{
			return &this->node->data.value;
		}

		const key_type&
		operator*() const
		{
			return this->node->data.key;
		}

		const key_type&
		key() const
		{
			return this->node->data.key;
		}

		const value_type&
		value() const
		{
			return this->node->data.value;
		}
	};

	template<typename T>
	T
	next(T it, usize n = 1)
	{
		while(n--)
			++it;
		return it;
	}

	template<typename T>
	T
	prev(T it, usize n = 1)
	{
		while(n--)
			--it;
		return it;
	}

	template<typename T>
	sequential_iterator<T>
	next(sequential_iterator<T> it, usize n = 1)
	{
		it._element += n;
		return it;
	}

	template<typename T>
	sequential_iterator<T>
	prev(sequential_iterator<T> it, usize n = 1)
	{
		it._element -= n;
		return it;
	}
}
