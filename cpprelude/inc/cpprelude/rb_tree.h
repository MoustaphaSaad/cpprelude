#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/tmp.h"
#include "cpprelude/allocator.h"
#include "cpprelude/iterator.h"
#include "cpprelude/queue_array.h"
#include <functional>


namespace cpprelude
{
	template<typename T, typename AllocatorT = global_allocator>
	struct rb_tree 
	{
		using RB_Node = details::rb_node<T>;
		using rb_iterator = rb_tree_iterator<T>;
		using const_rb_iterator = const rb_tree_iterator<T>;
		using COLOR = details::color_type;
		using data_type =T;

		RB_Node* _root;
		usize _count;
		AllocatorT _allocator;

		rb_tree(const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator), _root(nullptr)
		{}

		rb_tree(std::initializer_list<T> list, const AllocatorT& allocator = AllocatorT())
			:_count(list.size()), _allocator(allocator)
		{
			auto it = list.begin();
			for (usize i = 0; i < _count; ++i)
			{
				insert(*it);
				it = std::next(it);
			}
		}

		rb_tree(const rb_tree<T>& other)
			:_root(nullptr), _count(0), _allocator(other._allocator)
		{
			//auto self = const_cast<_RB_tree<T>*>(this);
			/*auto copy = [&](rb_iterator it)
			{
				insert(it->data);
			};
			other.inorder_traverse(copy);*/

			_copy_content(other);
		}
		
		rb_tree(const rb_tree<T>& other,const AllocatorT& allocator)
			:_root(nullptr), _count(0), _allocator(allocator)
		{
			_copy_content(other);
		}

		~rb_tree()
		{
			//_reset();
			clear();
		}

		rb_tree<T>&
		operator=(const rb_tree<T>& other)
		{
			clear();
			_allocator = other._allocator;
			_copy_content(other);
			return *this;
		}

		void 
		_free_mem(rb_iterator it)
		{
			if (it == nullptr) return;

			it->data.~T();
			_allocator.free(make_slice(&(*it)));
			--_count;
		}

		void
		clear()
		{
			_reset(rb_iterator(_root));
			_root = nullptr;
			_count = 0;
			_allocator = AllocatorT();
		}

		void
		_reset(rb_iterator it)
		{
			if (it == nullptr)
				return;

			if (it->left != nullptr)
				_reset(it->left);

			if (it->right != nullptr)
				_reset(it->right);

			_free_mem(it);
		}

		void 
		_copy_content(const rb_tree<T>& other)
		{
			queue_array<rb_iterator> queue;
			queue.enqueue(other.get_root());

			while (!queue.empty())
			{
				auto it = queue.front();
				queue.dequeue();
				if (it == nullptr) continue;
				
				insert(it->data);
				queue.enqueue(it->left);
				queue.enqueue(it->right);
			}
		}

		//modify if exisits, and insert if not.
		rb_iterator 
		insert(const T& key)
		{
			auto it_new_node =_create_node(key);
			if (_root == nullptr)
			{
				_root = &(*it_new_node);
				_recolor(it_new_node);
				++_count;
				return it_new_node;
			}
			else
			{
				auto temp = _insert(_root, *it_new_node);
				_reset_root();
				++_count;
				return temp;
			}
		}
		
		void 
		_reset_root()
		{
			if (_root == nullptr) return;
			while (_root->parent != nullptr)
				_root = _root->parent;
		}

		rb_iterator 
		lookup(const T& key)
		{
			rb_iterator result;
			if (_root != nullptr)
				result = _lookup(key, _root);
			return result;
		}

		const_rb_iterator
		lookup(const T& key) const
		{
			const_rb_iterator result;
			if (_root != nullptr)
				result = _lookup(key, _root);
			return result;
		}

		rb_iterator
		operator[](const T& key)
		{	//this function either returns the existed node or the new inserted node
			return insert(key);
		}

		template<typename function_type>
		void 
		inorder_traverse(function_type FT) 
		{
			_inorder_traverse(FT, _root);
		}

		template<typename function_type>
		void
		postorder_traverse(function_type FT) 
		{
			_postorder_traverse(FT, _root);
		}

		template<typename function_type>
		void
		preorder_traverse(function_type FT)
		{
			_preorder_traverse(FT, _root);
		}

		void
		_recolor(rb_iterator it)
		{
			//if this node is the root
			if (it->parent == nullptr)
			{
				it->color = COLOR::BLACK;
				return;
			}

			bool is_black_parent = it->parent->color == COLOR::BLACK;
			//If yes, then no violation to the rules 
			//Else parent is red and two red adjacent node exist.
			//hence, violation to the rules
			if (is_black_parent) return;

			//if parent is REd then it has grandpa
			bool is_parent_on_left_branch = it->parent->parent->data > it->parent->data;

			//is the parent node on the left branch from grandparent node or 
			//on the right branch.
			if (is_parent_on_left_branch)
			{
				auto grandpa_node = it->parent->parent;
				auto uncle_node = grandpa_node->right;
				bool is_uncle_red = uncle_node != nullptr && uncle_node->color == COLOR::RED;
				//if uncle exists and is red then recolor only.
				if (is_uncle_red)
				{
					//parent and uncle nodes become black
					uncle_node->color = it->parent->color = COLOR::BLACK;
					//grandparent becomes red
					grandpa_node->color = COLOR::RED;
					//check coloring starting from grandparent to root
					//grandpa_node->recolor();
					_recolor(rb_iterator(grandpa_node));
				}
				else // uncle is Black and rotation is required.
				{
					bool is_child_on_right_branch = it->data > it->parent->data;
					auto temp_parent = it->parent;
					// case 2: Left parent and right node
					//then it become case 1
					if (is_child_on_right_branch)
					{
						//left rotation at parent
						grandpa_node->left = &(*it);
						it->parent->parent = &(*it);
						it->parent->right = it->left;
						it->left = it->parent;
						it->parent = grandpa_node;

						temp_parent = &(*it);
					}

					//case 1: Left parent and left child node
					_rotate_left_left(grandpa_node, temp_parent);
				}
			}
			else // the node is in the right branch of the tree
			{
				auto grandpa_node = it->parent->parent;
				auto uncle_node = grandpa_node->left;
				bool is_uncle_red = uncle_node != nullptr && uncle_node->color == COLOR::RED;
				//if uncle exists and is red then recolor only.
				if (is_uncle_red)
				{
					//parent and uncle nodes become black
					uncle_node->color = it->parent->color = COLOR::BLACK;
					//grandparent becomes red
					grandpa_node->color = COLOR::RED;
					//check coloring starting from grandparent to root
					//grandpa_node->recolor();
					_recolor(rb_iterator(grandpa_node));
				}
				else // uncle is Black and rotation is required.
				{
					bool is_child_on_left_branch = it->data < it->parent->data;
					auto temp_parent = it->parent;

					// case 2: right parent and left node
					//fix it to become case 1
					if (is_child_on_left_branch)
					{
						//right rotation at parent
						grandpa_node->right = &(*it);
						it->parent->parent = &(*it);
						it->parent->left = it->right;
						it->right = it->parent;
						it->parent = grandpa_node;

						temp_parent = &(*it);
					}
					//case 1: Right parent and right child node
					_rotate_right_right(grandpa_node, temp_parent);
				}
			}
		}

		void
		_rotate_left_left(RB_Node* grandpa, RB_Node* parent)
		{
			//Right rotation at grandparent
			parent->parent = grandpa->parent;
			if (grandpa->parent != nullptr)
			{
				if (grandpa->parent->data < grandpa->data)
					grandpa->parent->right = parent;
				else
					grandpa->parent->left = parent;
			}
			grandpa->parent = parent;
			grandpa->left = parent->right;
			parent->right = grandpa;
			//swap colors of grandparent and parent
			tmp::swap(grandpa->color, parent->color);
		}

		void
		_rotate_right_right(RB_Node* grandpa, RB_Node* parent)
		{
			//Right rotation at grandparent
			parent->parent = grandpa->parent;
			if (grandpa->parent != nullptr)
			{
				if (grandpa->parent->data < grandpa->data)
					grandpa->parent->right = parent;
				else
					grandpa->parent->left = parent;
			}
			grandpa->parent = parent;
			grandpa->right = parent->left;
			parent->left = grandpa;
			//swap colors of grandparent and parent
			tmp::swap(grandpa->color, parent->color);
		}

		rb_iterator
		_create_node(const T& k)
		{
			auto temp = _allocator.template alloc<RB_Node>();
			new (temp) RB_Node(k);
			return rb_iterator(temp);
		}

		rb_iterator
		_insert(RB_Node* it,const RB_Node& new_node)
		{
			if (new_node.data < it->data)
			{
				if (it->left != nullptr)
					return _insert(it->left, new_node);
				else
				{
					it->left = _allocator.template alloc<RB_Node>();
					new (it->left) RB_Node(new_node.data, it);
					_recolor(it->left);
					return rb_iterator(it->left);
				}
			}
			else if (new_node.data > it->data)
			{
				if (it->right != nullptr)
					return _insert(it->right, new_node);
				else
				{
					it->right = _allocator.template alloc<RB_Node>();
					new (it->right) RB_Node(new_node.data, it);
					_recolor(it->right);
					return rb_iterator(it->right);
				}
			}
			else
				return rb_iterator(it);
		}

		rb_iterator
		_insert(RB_Node* it, RB_Node&& new_node)
		{
			if (new_node.data < it->data)
			{
				if (it->left != nullptr)
					return _insert(it->left, tmp::move(new_node));
				else
				{
					it->left = _allocator.template alloc<RB_Node>();
					new (it->left) RB_Node(new_node.data, it);
					_recolor(it->left);
					return rb_iterator(it->left);
				}
			}
			else if (new_node.data > it->data)
			{
				if (it->right != nullptr)
					return _insert(it->right, tmp::move(new_node));
				else
				{
					it->right = _allocator.template alloc<RB_Node>();
					new (it->right) RB_Node(new_node.data, it);
					_recolor(it->right);
					return rb_iterator(it->right);
				}
			}
			else
				return rb_iterator(it);
		}

		void
		delete_rb_tree(const T&k)
		{
			RB_Node* node_to_delete = &(*lookup(k));
			
			if (node_to_delete == nullptr) return;

			RB_Node* y = nullptr, *x = nullptr, *x_parent = nullptr;

			if (node_to_delete->left == nullptr || node_to_delete->right == nullptr)
				y = node_to_delete;
			else
				y = _get_successor(node_to_delete);

			if (y->left != nullptr)
				x = y->left;
			else
				x = y->right;

			if (x != nullptr)
				x->parent = y->parent;
			
			x_parent = y->parent;

			if (y->parent == nullptr)
			{
				_root = x;
				if (x != nullptr)
					x->color = COLOR::BLACK;
			}
			else if (y == y->parent->left)
				y->parent->left = x;
			else
				y->parent->right = x;

			if (y != node_to_delete)
				node_to_delete->data = y->data;

			if (y->color == COLOR::BLACK)
					_rb_delete_fixup(x, x_parent);

			_free_mem(rb_iterator(y));
		}

		void
		delete_rb_tree(rb_iterator it)
		{
			RB_Node* node_to_delete = &(*it);

			if (node_to_delete == nullptr) return;

			RB_Node* y = nullptr, *x = nullptr, *x_parent = nullptr;

			if (node_to_delete->left == nullptr || node_to_delete->right == nullptr)
				y = node_to_delete;
			else
				y = _get_successor(node_to_delete);

			if (y->left != nullptr)
				x = y->left;
			else
				x = y->right;

			if (x != nullptr)
				x->parent = y->parent;

			x_parent = y->parent;

			if (y->parent == nullptr)
			{
				_root = x;
				if (x != nullptr)
					x->color = COLOR::BLACK;
			}
			else if (y == y->parent->left)
				y->parent->left = x;
			else
				y->parent->right = x;

			if (y != node_to_delete)
				node_to_delete->data = y->data;

			if (y->color == COLOR::BLACK)
				_rb_delete_fixup(x, x_parent);

			_free_mem(rb_iterator(y));
		}

		RB_Node*
		_get_successor(RB_Node* node)
		{
			RB_Node* res = node->right;
			//get min value on the right branch
			// if it has no right branch, then check parents
			if (res != nullptr)
			{
				while (res->left != nullptr)
				{
					res = res->left;
				}
				return res;
			}
			
			res = node->parent;
			while (res != nullptr && node == res->right)
			{
				node = res;
				res = res->parent;
			}
			return res;
		}

		void
		_rb_delete_fixup(RB_Node* node, RB_Node* node_parent)
		{
			RB_Node* w = nullptr;
			bool is_node_black = true;
			if (node != nullptr)
				is_node_black = node->color == COLOR::BLACK;

			while (node != _root && is_node_black)
			{
				if (node == node_parent->left)
				{
					w = node_parent->right;
					if (w->color == COLOR::RED)
					{	//Case 1
						w->color = COLOR::BLACK;
						node_parent->color = COLOR::RED;
						_general_left_rotation(node_parent);
						w = node_parent->right;
					}
					bool is_w_left_black = true;
					bool is_w_right_black = true;
					if (w->left != nullptr)
						is_w_left_black = w->left->color == COLOR::BLACK;
					if (w->right != nullptr)
						is_w_right_black = w->right->color == COLOR::BLACK;

					if (is_w_left_black	&& is_w_right_black)
					{ //Case 2: black sibling with black childern
						w->color = COLOR::RED;
						node = node_parent;
						node_parent = node->parent;
						is_node_black = node->color == COLOR::BLACK;
					}
					else 
					{
						if (is_w_right_black)
						{	//Case 3
							w->left->color = COLOR::BLACK;
							w->color = COLOR::RED;
							_general_right_rotation(w);
							w = node_parent->right;
						}
						//Case 4
						w->color = node_parent->color;
						node_parent->color = COLOR::BLACK;
						if (w->right != nullptr)
							w->right->color = COLOR::BLACK;
						_general_left_rotation(node_parent);
						node = _root;
						node_parent = nullptr;
					}

				}
				else
				{
					w = node_parent->left;
					if (w->color == COLOR::RED)
					{	//Case 1
						w->color = COLOR::BLACK;
						node_parent->color = COLOR::RED;
						_general_right_rotation(node_parent);
						w = node_parent->left;
					}

					bool is_w_left_black = true;
					bool is_w_right_black = true;
					if (w->left != nullptr)
						is_w_left_black = w->left->color == COLOR::BLACK;
					if (w->right != nullptr)
						is_w_right_black = w->right->color == COLOR::BLACK;

					if (is_w_left_black	&& is_w_right_black)
					{	//Case 2
						w->color = COLOR::RED;
						node = node_parent;
						node_parent = node->parent;
						is_node_black = node->color == COLOR::BLACK;
					}
					else
					{
						if (is_w_left_black)
						{	//Case 3
							w->right->color = COLOR::BLACK;
							w->color = COLOR::RED;
							_general_left_rotation(w);
							w = node_parent->left;
						}
						//Case 4
						w->color = node_parent->color;
						node_parent->color = COLOR::BLACK;
						if (w->left != nullptr)// check this
							w->left->color = COLOR::BLACK;
						_general_right_rotation(node_parent);
						node = _root;
						node_parent = nullptr;
					}
				}
			}
			if (node != nullptr)
				node->color = COLOR::BLACK;
		}

		void
		_general_left_rotation(RB_Node* x)
		{
			RB_Node* y = x->right;
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
		_general_right_rotation(RB_Node* x)
		{
			RB_Node* y = x->left;
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

		RB_Node*
		_delete_bst(const T& k)
		{
			RB_Node* node_to_delete = &(*lookup(k));

			if (node_to_delete == nullptr) return _root;
			
			bool is_root = (node_to_delete == _root);
			bool has_left = node_to_delete->left != nullptr;
			bool has_right = node_to_delete->right != nullptr;
			bool parent_on_left = node_to_delete->parent != nullptr 
									&& node_to_delete->parent->data > node_to_delete->data;

			if (has_left && has_right)
			{
				//get median
				RB_Node* temp = node_to_delete->right;
				while (temp->left != nullptr)
					temp = temp->left;
				//deleting data
				tmp::swap(node_to_delete->data, temp->data);
				auto p = temp->parent; 
				if (p == node_to_delete)
					node_to_delete->right = nullptr;
				else p->left = nullptr;
				_free_mem(rb_iterator(temp));
				
			}
			else if (is_root)
			{
				_root = node_to_delete->left == nullptr ? node_to_delete->right : node_to_delete->left;
				if(_root!= nullptr) _root->parent = nullptr;
				_free_mem(rb_iterator(node_to_delete));
				node_to_delete = nullptr;
			}
			else if (!has_left)
			{
				//parent is on left branch
				if (node_to_delete->parent->data > node_to_delete->data)
				{
					node_to_delete->parent->left = node_to_delete->right;
					node_to_delete->right->parent = node_to_delete->parent;
				}
				//parent is on right branch
				else if(node_to_delete->parent->data < node_to_delete->data)
				{
					node_to_delete->parent->right = node_to_delete->right;
					node_to_delete->right->parent = node_to_delete->parent;
				}
				
				_free_mem(rb_iterator(node_to_delete));
				node_to_delete = nullptr;
			}
			else if (!has_right)
			{	//parent is on left branch
				if (node_to_delete->parent->data > node_to_delete->data)
				{
					node_to_delete->parent->left = node_to_delete->left;
					node_to_delete->left->parent = node_to_delete->parent;
				}
				//parent is on right branch
				else if (node_to_delete->parent->data < node_to_delete->data)
				{
					node_to_delete->parent->right = node_to_delete->left;
					node_to_delete->left->parent = node_to_delete->parent;
				}

				_free_mem(rb_iterator(node_to_delete));
				node_to_delete = nullptr;
			}
			else
			{
				//leaf node
				if (node_to_delete->parent->left == node_to_delete)
					node_to_delete->parent->left = nullptr;
				else
					node_to_delete->parent->right = nullptr;

				_free_mem(rb_iterator(node_to_delete));
				node_to_delete = nullptr;
			}
			return _root;
		}

		template<typename function_type>
		void
		_inorder_traverse(function_type fT,rb_iterator it) 
		{
			if (it->left != nullptr)
				_inorder_traverse(fT, it->left);

			fT(it);

			if (it->right != nullptr)
				_inorder_traverse(fT, it->right);
		}

		template<typename function_type>
		void
		_postorder_traverse(function_type fT, rb_iterator it) 
		{
			if (it->left != nullptr)
				_postorder_traverse(fT, it->left);

			if (it->right != nullptr)
				_postorder_traverse(fT, it->right);

			fT(it);
		}

		template<typename function_type>
		void
		_preorder_traverse(function_type fT, rb_iterator it)
		{
			fT(it);

			if (it->left != nullptr)
				_preorder_traverse(fT, it->left);

			if (it->right != nullptr)
				_preorder_traverse(fT, it->right);
		}
		
		rb_iterator
		_lookup(const T& k, RB_Node* it) 
		{
			if (k < it->data)
			{
				if (it->left != nullptr)
					return _lookup(k, it->left);

				else
					return rb_iterator(it->left);
			}
			else if (k > it->data)
			{
				if (it->right != nullptr)
					return _lookup(k, it->right);
				else
					return rb_iterator(it->right);
			}
			else
				return rb_iterator(it);
		}

		const_rb_iterator
		_lookup(const T& k, RB_Node* it) const
		{
			if (k < it->data)
			{
				if (it->left != nullptr)
					return _lookup(k, it->left);

				else
					return const_rb_iterator(it->left);
			}
			else if (k > it->data)
			{
				if (it->right != nullptr)
					return _lookup(k, it->right);
				else
					return const_rb_iterator(it->right);
			}
			else
				return const_rb_iterator(it);
		}

		bool
		is_rb_tree()
		{
			return (-1 != _calculate_Black_count(_root));
		}

		isize
		_calculate_Black_count(RB_Node* it)
		{
			if (it == nullptr) return 0;

			isize left_count = _calculate_Black_count(it->left);
			isize right_count = _calculate_Black_count(it->right);

			if (left_count == -1 || right_count == -1 || right_count != left_count)
				return -1;
			else
				return (it->color == COLOR::RED) ? left_count : left_count + 1;
		}

		void
		swap(rb_tree& other)
		{
			tmp::swap(_root, other._root);
			tmp::swap(_count, other._count);
			tmp::swap(_allocator, other._allocator);
		}

		rb_iterator
		get_min()
		{
			auto min = _root;
			if (min == nullptr) return rb_iterator();

			while (min->left != nullptr)
			{
				min = min->left;
			}
			return min;
		}

		rb_iterator
		get_max()
		{
			auto max = _root;
			if (max == nullptr) return rb_iterator();

			while (max->right != nullptr)
			{
				max = max->right;
			}
			return max;
		}

		rb_iterator
		get_root() const
		{
			return rb_iterator(_root);
		}

		usize
		count()
		{
			return _count;
		}
	};
}
