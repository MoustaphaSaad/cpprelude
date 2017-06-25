#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"

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

			pair_node(const key_type& k, const value_type& v)
				:key(k), value(v)
			{}
		};
		enum color_type { RED, BLACK };
		template<typename T>
		struct rb_node
		{
			
			T data;
			color_type color;
			rb_node<T> * left;
			rb_node<T> * right;
			rb_node<T> * parent;

			rb_node()
				:left(nullptr), right(nullptr), parent(nullptr), color (RED)
			{}

			rb_node(const T& d, color_type c = RED)
				:data(d), left(nullptr), right(nullptr), parent(nullptr), color(c)
			{}

			rb_node(const T& d,  rb_node<T>* p, color_type c = RED )
				:data(d), left(nullptr), right(nullptr), parent(p), color(c)
			{}

			rb_node(const T& d, rb_node<T>* p, rb_node<T>* l, rb_node<T>* r,  color_type c = RED)
				:data(d), left(l), right(r), parent(p), color(c)
			{}
		};
	}

	template<typename T>
	struct sequential_iterator
	{
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
	};

	template<typename T>
	struct forward_iterator
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
	struct bidirectional_iterator
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

	template<typename T, usize bucket_size>
	struct bucket_array_iterator
	{
		using data_type = T;
		T** _bucket_it;
		T* _element_it;
		usize _index;

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
	};

	template<typename T>
	struct rb_tree_iterator
	{
		using data_type = T;
		using RB_NODE = details::rb_node<T>;
		RB_NODE* _node;

		rb_tree_iterator()
			:_node(nullptr)
		{}
		
		rb_tree_iterator(RB_NODE* n)
			:_node(n)
		{}

		rb_tree_iterator(T* n)
			:_node(n)
		{}
		
		RB_NODE*
		get_left()
		{
			return _node->left;
		}

		RB_NODE*
		get_right()
		{
			return _node->right;
		}

		RB_NODE*
		get_parent()
		{
			return _node->parent;
		}

		details::color_type
		get_color() const
		{
			return _node->color;
		}

		void
		move_up()
		{
			_node = _node->parent;
		}

		void
		move_left()
		{
			_node = _node->left;
		}

		void
		move_right()
		{
			_node = _node->right;
		}

		RB_NODE*
		operator->()
		{
			return &(*_node);
		}

		const RB_NODE*
		operator->() const
		{
			return &(*_node);
		}

		const RB_NODE&
		operator*() const
		{
			return *_node;
		}

		RB_NODE&
		operator*()
		{
			return *_node;
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
