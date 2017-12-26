#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/iterator.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"
#include "cpprelude/memory.h"
#include "cpprelude/defaults.h"

//because of the idiots at microsoft
#undef min
#undef max

namespace cpprelude
{
	//red black tree implmenetion follows Introduction to Algorithms, Second Edition,” by Thomas H. Cormen, Charles E, Chapter 14

	template<typename T,
			 typename ComparatorType = default_less_than<T>>
	struct red_black_tree
	{
		using data_type = T;
		using node_type = details::red_black_tree_node<data_type>;
		using iterator = red_black_tree_iterator<T>;
		using const_iterator = red_black_tree_iterator<T>;
		using color_type = typename node_type::color_type;

		node_type *_root;
		usize _count;
		memory_context *_context = platform->global_memory;
		ComparatorType _less_than;

		red_black_tree(const ComparatorType& compare_function, memory_context* context = platform->global_memory)
			:_root(nullptr), _count(0), _context(context),
			 _less_than(compare_function)
		{}

		red_black_tree(memory_context* context = platform->global_memory)
			:_root(nullptr), _count(0), _context(context)
		{}

		red_black_tree(std::initializer_list<T> list,
			const ComparatorType& compare_function = ComparatorType(),
			memory_context* context = platform->global_memory)
			:_root(nullptr), _count(0), _context(context), _less_than(compare_function)
		{
			auto it = list.begin();
			for(usize i = 0;
				i < list.size();
				++i)
				{
					insert(*it);
					it = std::next(it);
				}
		}

		red_black_tree(const red_black_tree& other)
			:_root(nullptr), _count(0), _context(other._context),
			 _less_than(other._less_than)
		{
			_copy_content(other);
		}

		red_black_tree(const red_black_tree& other,
			const ComparatorType& compare_function)
			:_root(nullptr), _count(0), _context(other._context),
			 _less_than(compare_function)
		{
			_copy_content(other);
		}

		red_black_tree(const red_black_tree& other,
			memory_context* context)
			:_root(nullptr), _count(0), _context(context),
			 _less_than(other._less_than)
		{
			_copy_content(other);
		}

		red_black_tree(const red_black_tree& other,
			memory_context* context,
			const ComparatorType& compare_function)
			:_root(nullptr), _count(0), _context(context),
			 _less_than(compare_function)
		{
			_copy_content(other);
		}

		red_black_tree(red_black_tree&& other)
			:_root(other._root), _count(other._count),
			 _context(other._context),
			 _less_than(std::move(other._less_than))
		{
			other._root = nullptr;
			other._count = 0;
			other._context = nullptr;
		}

		red_black_tree(red_black_tree&& other, const ComparatorType& compare_function)
			:_root(other._root), _count(other._count),
			 _context(other._context),
			 _less_than(compare_function)
		{
			other._root = nullptr;
			other._count = 0;
			other._context = nullptr;
		}

		red_black_tree(red_black_tree&& other, memory_context* context)
			:_root(other._root), _count(other._count),
			 _context(context),
			 _less_than(std::move(other._less_than))
		{
			other._root = nullptr;
			other._count = 0;
			other._context = nullptr;
		}

		red_black_tree(red_black_tree&& other, memory_context* context,
			const ComparatorType& compare_function)
			:_root(other._root), _count(other._count),
			 _context(context),
			 _less_than(compare_function)
		{
			other._root = nullptr;
			other._count = 0;
			other._context = nullptr;
		}

		~red_black_tree()
		{
			clear();
			_context = nullptr;
		}

		red_black_tree&
		operator=(const red_black_tree& other)
		{
			clear();
			_context = other._context;
			_less_than = other._less_than;
			_copy_content(other);
			return *this;
		}

		red_black_tree&
		operator=(red_black_tree&& other)
		{
			_reset(_root);
			_context = std::move(other._context);
			_less_than = std::move(other._less_than);

			_count = other._count;
			_root = other._root;
			other._count = 0;
			other._root = nullptr;
			other._context = nullptr;
			return *this;
		}

		void
		clear()
		{
			_reset(_root);
			_root = nullptr;
			_count = 0;
		}

		iterator
		insert(const T& data)
		{
			return iterator(_insert(data));
		}

		iterator
		insert(T&& data)
		{
			return iterator(_insert(std::move(data)));
		}

		void
		remove(const T& data)
		{
			remove(lookup(data));
		}

		void
		remove(T&& data)
		{
			remove(lookup(data));
		}

		void
		remove(iterator node_to_delete)
		{
			if(node_to_delete == end())
				return;

			node_type *x = nullptr, *x_parent = nullptr;
			iterator y;

			if (node_to_delete.node->left == nullptr || node_to_delete.node->right == nullptr)
				y = node_to_delete;
			else
			{
				y = node_to_delete;
				++y;
			}

			if (y.node->left != nullptr)
				x = y.node->left;
			else
				x = y.node->right;

			if (x != nullptr)
				x->parent = y.node->parent;

			x_parent = y.node->parent;

			if (y.node->parent == nullptr)
			{
				_root = x;
				if (x != nullptr)
					x->color = color_type::BLACK;
			}
			else if (y.node == y.node->parent->left)
				y.node->parent->left = x;
			else
				y.node->parent->right = x;

			if (y != node_to_delete)
				const_cast<node_type*>(node_to_delete.node)->data = *y;

			if (y.node->color == color_type::BLACK)
				_rb_delete_fixup(x, x_parent);

			_free_mem(const_cast<node_type*>(y.node));
		}

		iterator
		lookup(const T& data)
		{
			node_type* result = _root;
			if(result != nullptr)
				result = _lookup(data, _root);
			return iterator(result);
		}

		iterator
		lookup(T&& data)
		{
			return lookup(data);
		}

		const_iterator
		lookup(const T& data) const
		{
			node_type* result = _root;
			if(result != nullptr)
				result = _lookup(data, _root);
			return const_iterator(result);
		}

		const_iterator
		lookup(T&& data) const
		{
			return lookup(data);
		}

		iterator
		root()
		{
			return iterator(_root);
		}

		const_iterator
		root() const
		{
			return const_iterator(_root);
		}

		template<typename function_type, typename user_type = void>
		void
		inorder_traverse(function_type&& FT, user_type* user_data = nullptr)
		{
			_inorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		template<typename function_type, typename user_type = void>
		void
		postorder_traverse(function_type&& FT, user_type* user_data = nullptr)
		{
			_postorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		template<typename function_type, typename user_type = void>
		void
		preorder_traverse(function_type&& FT, user_type* user_data = nullptr)
		{
			_preorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		template<typename function_type, typename user_type = void>
		void
		inorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
		{
			_inorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		template<typename function_type, typename user_type = void>
		void
		postorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
		{
			_postorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		template<typename function_type, typename user_type = void>
		void
		preorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
		{
			_preorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		void
		swap(red_black_tree& other)
		{
			std::swap(_root, other._root);
			std::swap(_count, other._count);
			std::swap(_context, other._context);
			std::swap(_less_than, other._less_than);
		}

		iterator
		min()
		{
			return iterator(_min());
		}

		const_iterator
		min() const
		{
			return const_iterator(_min());
		}

		iterator
		max()
		{
			return iterator(_max());
		}

		const_iterator
		max() const
		{
			return const_iterator(_max());
		}

		iterator
		begin()
		{
			return min();
		}

		const_iterator
		begin() const
		{
			return min();
		}

		const_iterator
		cbegin() const
		{
			return min();
		}

		iterator
		end()
		{
			return nullptr;
		}

		const_iterator
		end() const
		{
			return nullptr;
		}

		const_iterator
		cend() const
		{
			return nullptr;
		}

		usize
		count() const
		{
			return _count;
		}

		usize
		empty() const
		{
			return _count == 0;
		}

		node_type*
		_min() const
		{
			auto m = _root;
			if (m == nullptr)
				return nullptr;

			while (m->left != nullptr)
			{
				m = m->left;
			}
			return m;
		}

		node_type*
		_max() const
		{
			auto m = _root;
			if (m == nullptr)
				return nullptr;

			while (m->right != nullptr)
			{
				m = m->right;
			}
			return m;
		}

		bool
		_is_red_black_tree() const
		{
			return _is_rb(_root) != -1;
		}

		node_type*
		_insert(const T& data)
		{
			node_type* y = nullptr;
			node_type* x = _root;

			while (x != nullptr)
			{
				y = x;
				if (_less_than(data, x->data))
					x = x->left;
				else if (_less_than(x->data, data))
					x = x->right;
				else
				{
					return x;
				}
			}
			++_count;
			node_type* z = _create_node(data);
			z->parent = y;
			if (y == nullptr)
				_root = z;
			else
			{
				if (_less_than(z->data, y->data))
					y->left = z;
				else
					y->right = z;
			}
			_insert_fixup(z);
			return z;
		}

		node_type*
		_insert(T&& data)
		{
			node_type* y = nullptr;
			node_type* x = _root;

			while (x != nullptr)
			{
				y = x;
				if (_less_than(data, x->data))
					x = x->left;
				else if (_less_than(x->data, data))
					x = x->right;
				else
				{
					return x;
				}
			}
			++_count;
			node_type* z = _create_node(std::move(data));
			z->parent = y;
			if (y == nullptr)
				_root = z;
			else
			{
				if (_less_than(z->data, y->data))
					y->left = z;
				else
					y->right = z;
			}
			_insert_fixup(z);
			return z;
		}

		void
		_insert_fixup(node_type* z)
		{
			auto is_red = [](node_type* p) { return p != nullptr && p->color == color_type::RED; };

			while (is_red(z->parent))
			{
				if (z->parent == z->parent->parent->left)
				{
					node_type* y = z->parent->parent->right;
					if (is_red(y))
					{ //Casse 1
						z->parent->color = color_type::BLACK;
						y->color = color_type::BLACK;
						z->parent->parent->color = color_type::RED;
						z = z->parent->parent;
					}
					else
					{
						if (z == z->parent->right)
						{ //case 2
							z = z->parent;
							_left_rotation(z);
						}
						//case 3
						z->parent->color = color_type::BLACK;
						z->parent->parent->color = color_type::RED;
						_right_rotation(z->parent->parent);
					}
				}
				else
				{// right instead of left

					node_type* y = z->parent->parent->left;
					if (is_red(y))
					{ //Casse 1
						z->parent->color = color_type::BLACK;
						y->color = color_type::BLACK;
						z->parent->parent->color = color_type::RED;
						z = z->parent->parent;
					}
					else
					{
						if (z == z->parent->left)
						{ //case 2
							z = z->parent;
							_right_rotation(z);
						}
						//case 3
						z->parent->color = color_type::BLACK;
						z->parent->parent->color = color_type::RED;
						_left_rotation(z->parent->parent);
					}
				}
			}
			_root->color = color_type::BLACK;
		}

		void
		_rb_delete_fixup(node_type* node, node_type* node_parent)
		{
			node_type* w = nullptr;
			bool is_node_black = true;
			if (node != nullptr)
				is_node_black = node->color == color_type::BLACK;

			while (node != _root && is_node_black)
			{
				if (node == node_parent->left)
				{
					w = node_parent->right;
					if (w->color == color_type::RED)
					{	//Case 1
						w->color = color_type::BLACK;
						node_parent->color = color_type::RED;
						_left_rotation(node_parent);
						w = node_parent->right;
					}
					bool is_w_left_black = true;
					bool is_w_right_black = true;
					if (w->left != nullptr)
						is_w_left_black = w->left->color == color_type::BLACK;
					if (w->right != nullptr)
						is_w_right_black = w->right->color == color_type::BLACK;

					if (is_w_left_black	&& is_w_right_black)
					{ //Case 2: black sibling with black childern
						w->color = color_type::RED;
						node = node_parent;
						node_parent = node->parent;
						is_node_black = node->color == color_type::BLACK;
					}
					else
					{
						if (is_w_right_black)
						{	//Case 3
							w->left->color = color_type::BLACK;
							w->color = color_type::RED;
							_right_rotation(w);
							w = node_parent->right;
						}
						//Case 4
						w->color = node_parent->color;
						node_parent->color = color_type::BLACK;
						if (w->right != nullptr)
							w->right->color = color_type::BLACK;
						_left_rotation(node_parent);
						node = _root;
						node_parent = nullptr;
					}

				}
				else
				{
					w = node_parent->left;
					if (w->color == color_type::RED)
					{	//Case 1
						w->color = color_type::BLACK;
						node_parent->color = color_type::RED;
						_right_rotation(node_parent);
						w = node_parent->left;
					}

					bool is_w_left_black = true;
					bool is_w_right_black = true;
					if (w->left != nullptr)
						is_w_left_black = w->left->color == color_type::BLACK;
					if (w->right != nullptr)
						is_w_right_black = w->right->color == color_type::BLACK;

					if (is_w_left_black	&& is_w_right_black)
					{	//Case 2
						w->color = color_type::RED;
						node = node_parent;
						node_parent = node->parent;
						is_node_black = node->color == color_type::BLACK;
					}
					else
					{
						if (is_w_left_black)
						{	//Case 3
							w->right->color = color_type::BLACK;
							w->color = color_type::RED;
							_left_rotation(w);
							w = node_parent->left;
						}
						//Case 4
						w->color = node_parent->color;
						node_parent->color = color_type::BLACK;
						if (w->left != nullptr)// check this
							w->left->color = color_type::BLACK;
						_right_rotation(node_parent);
						node = _root;
						node_parent = nullptr;
					}
				}
			}
			if (node != nullptr)
				node->color = color_type::BLACK;
		}

		void
		_left_rotation(node_type* x)
		{
			node_type* y = x->right;
			x->right = y->left;
			if (y->left != nullptr)
				y->left->parent = x;
			y->parent = x->parent;
			if (x->parent == nullptr)
				_root = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
			y->left = x;
			x->parent = y;
		}

		void
		_right_rotation(node_type* x)
		{
			node_type* y = x->left;
			x->left = y->right;
			if (y->right != nullptr)
				y->right->parent = x;
			y->parent = x->parent;
			if (x->parent == nullptr)
				_root = y;
			else if (x == x->parent->right)
				x->parent->right = y;
			else
				x->parent->left = y;
			y->right = x;
			x->parent = y;
		}

		node_type*
		_create_node(const T& data)
		{
			auto result = _context->template alloc<node_type>();
			new (result) node_type(data);
			return result;
		}

		node_type*
		_create_node(T&& data)
		{
			auto result = _context->template alloc<node_type>();
			new (result) node_type(std::move(data));
			return result;
		}

		void
		_free_mem(node_type* it)
		{
			if (it == nullptr) return;

			it->data.~data_type();
			if(_context) _context->free(make_slice(it));
			--_count;
		}

		void
		_reset(node_type* it)
		{
			if(it == nullptr)
				return;

			auto func = [](iterator it, red_black_tree* tree)
			{
				tree->_free_mem(const_cast<node_type*>(it.node));
			};
			_postorder_traverse(func, _root, this);
		}

		isize
		_is_rb(node_type* it) const
		{
			if (it == nullptr) return 0;

			isize left_count = _is_rb(it->left);
			isize right_count = _is_rb(it->right);

			if (left_count == -1 || right_count == -1 || right_count != left_count)
				return -1;
			else
				return (it->color == color_type::RED) ? left_count : left_count + 1;
		}

		void
		_copy_content(const red_black_tree& other)
		{
			if(other.empty())
				return;

			auto func = [](const_iterator it, red_black_tree* tree)
			{
				tree->insert(*it);
			};
			other.preorder_traverse(func, this);
		}

		template<typename function_type, typename user_type>
		void
		_inorder_traverse(function_type&& fT, iterator it, user_type* user_data)
		{
			if (it.node->left != nullptr)
				_inorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			fT(it, user_data);

			if (it.node->right != nullptr)
				_inorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);
		}

		template<typename function_type, typename user_type>
		void
		_postorder_traverse(function_type&& fT, iterator it, user_type* user_data)
		{
			if (it.node->left != nullptr)
				_postorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			if (it.node->right != nullptr)
				_postorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);

			fT(it, user_data);
		}

		template<typename function_type, typename user_type>
		void
		_preorder_traverse(function_type&& fT, iterator it, user_type* user_data)
		{
			fT(it, user_data);

			if (it.node->left != nullptr)
				_preorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			if (it.node->right != nullptr)
				_preorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);
		}

		template<typename function_type, typename user_type>
		void
		_inorder_traverse(function_type&& fT, const_iterator it, user_type* user_data) const
		{
			if (it.node->left != nullptr)
				_inorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			fT(it, user_data);

			if (it.node->right != nullptr)
				_inorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);
		}

		template<typename function_type, typename user_type>
		void
		_postorder_traverse(function_type&& fT, const_iterator it, user_type* user_data) const
		{
			if (it.node->left != nullptr)
				_postorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			if (it.node->right != nullptr)
				_postorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);

			fT(it, user_data);
		}

		template<typename function_type, typename user_type>
		void
		_preorder_traverse(function_type&& fT, const_iterator it, user_type* user_data) const
		{
			fT(it, user_data);

			if (it.node->left != nullptr)
				_preorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			if (it.node->right != nullptr)
				_preorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);
		}

		node_type*
		_lookup(const data_type& key, node_type* it) const
		{
			if (_less_than(key , it->data))
			{
				if (it->left != nullptr)
					return _lookup(key, it->left);

				else
					return it->left;
			}
			else if (_less_than(it->data, key))
			{
				if (it->right != nullptr)
					return _lookup(key, it->right);
				else
					return it->right;
			}
			else
				return it;
		}

		node_type*
		_lookup(data_type&& key, node_type* it) const
		{
			return _lookup(key, it);
		}
	};

	template<typename KeyType, typename ValueType,
		typename ComparatorType = default_less_than<details::pair_node<KeyType, ValueType>>>
	struct red_black_map: public red_black_tree<details::pair_node<KeyType, ValueType>, ComparatorType>
	{
		using key_type = KeyType;
		using value_type = ValueType;
		using data_type = details::pair_node<key_type, value_type>;
		using node_type = details::red_black_tree_node<data_type>;
		using iterator = red_black_tree_pair_iterator<key_type, value_type>;
		using const_iterator = red_black_tree_const_pair_iterator<key_type, value_type>;
		using color_type = typename node_type::color_type;
		using _implementation = red_black_tree<data_type, ComparatorType>;

		red_black_map(memory_context* context = platform->global_memory)
			:_implementation(context)
		{}

		red_black_map(const ComparatorType& compare_function, memory_context* context = platform->global_memory)
			:_implementation(compare_function, context)
		{}

		red_black_map(std::initializer_list<data_type> list,
			const ComparatorType& compare_function = ComparatorType(),
			memory_context* context = platform->global_memory)
			:_implementation(list, compare_function, context)
		{}

		red_black_map(const red_black_map& other)
			:_implementation(other)
		{}

		red_black_map(const red_black_map& other,
			const ComparatorType& compare_function)
			:_implementation(other, compare_function)
		{}

		red_black_map(const red_black_map& other,
			memory_context* context)
			:_implementation(other, context)
		{}

		red_black_map(const red_black_map& other,
			memory_context* context,
			const ComparatorType& compare_function)
			:_implementation(other, context, compare_function)
		{}

		red_black_map(red_black_map&& other)
			:_implementation(other)
		{}

		red_black_map(red_black_map&& other, const ComparatorType& compare_function)
			:_implementation(other, compare_function)
		{}

		red_black_map(red_black_map&& other, memory_context* context)
			:_implementation(other, context)
		{}

		red_black_map(red_black_map&& other, memory_context* context,
			const ComparatorType& compare_function)
			:_implementation(other, context, compare_function)
		{}

		value_type&
		operator[](const key_type& key)
		{
			return const_cast<value_type&>(_implementation::insert(data_type(key))->value);
		}

		const value_type&
		operator[](const key_type& key) const
		{
			return _implementation::insert(data_type(key))->value;
		}

		iterator
		insert(const key_type& key)
		{
			return _implementation::insert(data_type(key));
		}

		iterator
		insert(key_type&& key)
		{
			return _implementation::insert(data_type(std::move(key)));
		}

		iterator
		insert(const key_type& key, const value_type& value)
		{
			return _implementation::insert(data_type(key, value));
		}

		iterator
		insert(key_type&& key, const value_type& value)
		{
			return _implementation::insert(data_type(std::move(key), value));
		}

		iterator
		insert(const key_type& key, value_type&& value)
		{
			return _implementation::insert(data_type(key, std::move(value)));
		}

		iterator
		insert(key_type&& key, value_type&& value)
		{
			return _implementation::insert(data_type(std::move(key), std::move(value)));
		}

		using _implementation::insert;

		void
		remove(const key_type& key)
		{
			_implementation::remove(lookup(data_type(key)));
		}

		void
		remove(key_type&& key)
		{
			_implementation::remove(lookup(data_type(std::move(key))));
		}

		using _implementation::remove;

		iterator
		lookup(const key_type& key)
		{
			return _implementation::lookup(data_type(key));
		}

		iterator
		lookup(key_type&& key)
		{
			return _implementation::lookup(data_type(std::move(key)));
		}

		const_iterator
		lookup(const key_type& key) const
		{
			return _implementation::lookup(data_type(key));
		}

		const_iterator
		lookup(key_type&& key) const
		{
			return _implementation::lookup(data_type(std::move(key)));
		}

		using _implementation::lookup;

		iterator
		begin()
		{
			return _implementation::min();
		}

		const_iterator
		begin() const
		{
			return _implementation::min();
		}

		const_iterator
		cbegin() const
		{
			return _implementation::min();
		}

		iterator
		end()
		{
			return nullptr;
		}

		const_iterator
		end() const
		{
			return nullptr;
		}

		const_iterator
		cend() const
		{
			return nullptr;
		}
	};

	template<typename T,
			 typename ComparatorType = default_less_than<T>>
	using tree_set = red_black_tree<T, ComparatorType>;

	template<typename KeyType, typename ValueType,
		typename ComparatorType = default_less_than<details::pair_node<KeyType, ValueType>>>
	using tree_map = red_black_map<KeyType, ValueType, ComparatorType>;
}
