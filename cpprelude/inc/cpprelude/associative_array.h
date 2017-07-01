#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/tmp.h"
#include "cpprelude/allocator.h"
#include "cpprelude/iterator.h"
#include "cpprelude/rb_tree.h"

namespace cpprelude
{
	template<typename key_type, typename value_type, typename AllocatorT = global_allocator>
	struct associative_array
	{
		using pair_node = details::pair_node<key_type, value_type>;
		using data_type = pair_node;
		using rb_iterator = rb_tree_iterator<pair_node>;
		using const_rb_iterator = const rb_tree_iterator<pair_node>;
		rb_tree <pair_node, AllocatorT> _tree;
		
		associative_array(const AllocatorT& allocator = AllocatorT())
			:_tree(allocator)
		{}

		associative_array(std::initializer_list<pair_node> list, const AllocatorT& allocator = AllocatorT())
			: _tree(allocator)
		{
			auto it = list.begin();
			for (usize i = 0; i < _count; ++i)
			{
				tree.insert(*it);
				it = std::next(it);
			}
		}

		associative_array(const associative_array<key_type, value_type>& other)
			:_tree(other._tree, other._allocator)
		{
		}

		associative_array(const associative_array<key_type, value_type>& other, const AllocatorT& allocator)
			:_tree(other._tree, allocator)
		{
		}

		associative_array(associative_array<key_type, value_type>&& other, const AllocatorT& allocator)
			:_tree(tmp::move(other._tree), allocator)
		{
		}

		associative_array(associative_array<key_type, value_type>&& other)
			:_tree(tmp::move(other._tree))
		{
		}
		
		rb_iterator
		insert(const key_type& k)
		{
			return _tree.insert(pair_node(k));
		}

		rb_iterator
		insert(const key_type& k, const value_type& v)
		{
			return _tree.insert(pair_node(k, v));
		}

		rb_iterator
		insert(const pair_node & p)
		{
			return _tree.insert(p);
		}

		rb_iterator
		insert(key_type&& k)
		{
			return _tree.insert(tmp::move(pair_node(tmp::move(k))));
		}

		rb_iterator
		insert(key_type&& k, value_type&& v)
		{
			return _tree.insert(tmp::move(pair_node(tmp::move(k), tmp::move(v))));
		}

		rb_iterator
		insert(pair_node && p)
		{
			return _tree.insert(tmp::move(p));
		}


		void
		erase(rb_iterator it)
		{
			_tree.delete_rb_tree(it);
		}

		void
		erase(const key_type& k)
		{
			_tree.delete_rb_tree(k);
		}

		void
		swap(associative_array& other)
		{
			_tree.swap(other._tree);
		}

		void
		swap(associative_array&& other)
		{
			_tree.swap(tmp::move(other._tree));
		}

		rb_iterator
		find(const key_type& k)
		{
			return _tree.lookup(k);
		}

		const_rb_iterator
		find(const key_type& k) const
		{
			return _tree.lookup(k);
		}

		value_type&
		at(const key_type& k)
		{
			pair_node n(k);
			return _tree[n]->data.value;
		}

		const value_type&
		at(const key_type& k) const
		{
			pair_node n(k);
			return _tree[n]->data.value;
		}

		value_type&
		operator[](const key_type& key)
		{
			return at(key);
		}

		value_type
		operator[](const key_type& key) const
		{
			return at(key);
		}

		AllocatorT
		get_allocator()
		{
			return _tree._allocator;
		}

		rb_iterator
		get_min()
		{
			return _tree.get_min();
		}

		rb_iterator
		get_max()
		{
			return _tree.get_max();
		}

		void
		delete_min()
		{
			auto it = _tree.get_min();
			erase(it);
		}

		void
		delete_max()
		{
			auto it = _tree.get_max();
			erase(it);
		}

		bool
		empty()
		{
			return _tree.count() == 0;
		}

		void
		clear()
		{
			_tree.clear();
		}

		usize
		count()
		{
			return _tree.count();
		}
	};
}