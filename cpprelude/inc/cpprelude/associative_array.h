#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/tmp.h"
#include "cpprelude/allocator.h"
#include "cpprelude/iterator.h"

namespace cpprelude
{
	enum _color_type { RED, BLACK };

	template<typename key_type, typename AllocatorT = global_allocator>
	struct _RB_Node
	{
		using RB_Node = _RB_Node<key_type, AllocatorT>;
		using iterator = sequential_iterator<RB_Node>;
		using const_iterator = sequential_iterator<const RB_Node>;

		key_type key;
		//value_type value;
		_color_type color;
		RB_Node* left;
		RB_Node* right;
		RB_Node* parent;
		AllocatorT _allocator;

		_RB_Node(const AllocatorT& allocator = AllocatorT())
			:color(RED), _allocator(allocator),
			left(nullptr), right(nullptr), parent(nullptr)
		{}

		_RB_Node(key_type k, const AllocatorT& allocator = AllocatorT())
			:key(k), color(RED), _allocator(allocator),
			left(nullptr), right(nullptr), parent(nullptr)
		{}

		//_RB_Node(key_type k, /*value_type v,*/ const AllocatorT& allocator = AllocatorT())
		//	:key(k), /*value(v),*/ color(RED), _allocator(allocator),
		//	left(nullptr), right(nullptr), parent(nullptr)
		//{}

		_RB_Node(key_type k,/* value_type v,*/ RB_Node* p, const AllocatorT& allocator = AllocatorT())
			:key(k), /*value(v),*/ color(RED), _allocator(allocator),
			left(nullptr), right(nullptr), parent(p)
		{}

		void
		recolor()
		{
			//if this node is the root
			if (parent == nullptr)
			{
				color = BLACK;
				return;
			}

			bool is_black_parent = parent->color == BLACK;
			//If yes, then no violation to the rules 
			//Else parent is red and two red adjacent node exist.
			//hence, violation to the rules
			if (is_black_parent) return;

			//if parent is REd then it has grandpa
			bool is_parent_on_left_branch = parent->parent->key > parent->key;

			//is the parent node on the left branch from grandparent node or 
			//on the right branch.
			if (is_parent_on_left_branch)
			{
				auto grandpa_node = parent->parent;
				auto uncle_node = grandpa_node->right;
				bool is_uncle_red = uncle_node != nullptr && uncle_node->color == RED;
				//if uncle exists and is red then recolor only.
				if (is_uncle_red)
				{
					//parent and uncle nodes become black
					uncle_node->color = parent->color = BLACK;
					//grandparent becomes red
					grandpa_node->color = RED;
					//check coloring starting from grandparent to root
					grandpa_node->recolor();
				}
				else // uncle is Black and rotation is required.
				{
					bool is_child_on_right_branch = key > parent->key;
					auto temp_parent = parent;
					// case 2: Left parent and right node
					//then it become case 1
					if (is_child_on_right_branch)
					{
						//left rotation at parent
						grandpa_node->left = this;
						parent->parent = this;
						parent->right = this->left;
						this->left = parent;
						this->parent = grandpa_node;

						temp_parent = this;
					}

					//case 1: Left parent and left child node
					_rotate_left_left(grandpa_node, temp_parent);
				}
			}
			else // the node is in the right branch of the tree
			{
				auto grandpa_node = parent->parent;
				auto uncle_node = grandpa_node->left;
				bool is_uncle_red = uncle_node != nullptr && uncle_node->color == RED;
				//if uncle exists and is red then recolor only.
				if (is_uncle_red)
				{
					//parent and uncle nodes become black
					uncle_node->color = parent->color = BLACK;
					//grandparent becomes red
					grandpa_node->color = RED;
					//check coloring starting from grandparent to root
					grandpa_node->recolor();
				}
				else // uncle is Black and rotation is required.
				{
					bool is_child_on_left_branch = key < parent->key;
					auto temp_parent = parent;

					// case 2: right parent and left node
					//fix it to become case 1
					if (is_child_on_left_branch)
					{
						//right rotation at parent
						grandpa_node->right = this;
						parent->parent = this;
						parent->left = this->right;
						this->right = parent;
						this->parent = grandpa_node;

						temp_parent = this;
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
			grandpa->parent = parent;
			grandpa->right = parent->left;
			parent->left = grandpa;
			//swap colors of grandparent and parent
			tmp::swap(grandpa->color, parent->color);
		}

		iterator
		insert(const RB_Node & new_node)
		{

			if (new_node.key < key)
			{
				if (left != nullptr)
					return left->insert(new_node);

				else
				{
					left = _allocator.template alloc<RB_Node>();
					new (left) RB_Node(new_node.key, /*new_node.value,*/ this);
					left->recolor();
					return iterator(left);
				}
			}
			else if (new_node.key > key)
			{
				if (right != nullptr)
					return right->insert(new_node);
				else
				{
					right = _allocator.template alloc<RB_Node>();
					new (right) RB_Node(new_node.key, /*new_node.value,*/ this);
					right->recolor();
					return iterator(right);
				}
			}
			// Modify the value of this node
			else
			{
				//value = new_node.value;
				return iterator(this);
			}
		}

		static iterator
		create_node(const key_type& k)
		{
			auto temp = _allocator.template alloc<RB_Node>();
			new (temp) RB_Node(k);
			//temp->recolor();
			return iterator(temp);
		}
		//iterator
		//insert(_RB_Node<key_type>&& new_node)
		//{

		//	if (new_node.key < key)
		//	{
		//		if (left != nullptr)
		//			return left->insert(tmp::move(new_node));
		//		else
		//		{
		//			left = _allocator.template alloc<_RB_Node<key_type>>();
		//			new (left) _RB_Node<key_type>(new_node.key, new_node.value, this);
		//			left->recolor();
		//			return iterator(left);
		//		}
		//	}
		//	else if (new_node.key > key)
		//	{
		//		if (right != nullptr)
		//			return right->insert(tmp::move(new_node));
		//		else
		//		{
		//			right = _allocator.template alloc<_RB_Node<key_type>>();
		//			new (right) _RB_Node<key_type>(new_node.key, new_node.value, this);
		//			right->recolor();
		//			return iterator(right);
		//		}
		//	}
		//	// Modify the value of this node
		//	else
		//	{
		//		//value = tmp::move(new_node.value);
		//		return iterator(this);
		//	}
		//}

		////check if it exists and returns iterator to this node. 
		////if not exists an iterator with nullptr will be returned where it should be inserted
		iterator
		lookup(const key_type& k)
		{
			if (k < key)
			{
				if (left != nullptr)
					return left->lookup(k);

				else
					return iterator(left);
			}
			else if (k > key)
			{
				if (right != nullptr)
					return right->lookup(k);
				else
					return iterator(right);
			}
			else
				return iterator(this);
		}

		const_iterator
		lookup(const key_type& k) const
		{
			if (k < key)
			{
				if (left != nullptr)
					return left->lookup(k);

				else
					return iterator(left);
			}
			else if (k > key)
			{
				if (right != nullptr)
					return right->lookup(k);
				else
					return iterator(right);
			}
			else
				return iterator(this);
		}
		/*value_type&
		lookup(const key_type& k)
		{
		value_type* val= nullptr;

		if (k < key)
		{
		if (left != nullptr)
		val = &left->lookup(k);

		}
		else if (k > key)
		{
		if (right != nullptr)
		val = &right->lookup(k);
		}
		else val = &value;

		return *val;
		}
		*/
		template<typename function_type>
		void
		inorder_traverse(function_type fT) const
		{
			if (left != nullptr)
				left->inorder_traverse(fT);

			fT(*this);

			if (right != nullptr)
				right->inorder_traverse(fT);
		}
	};

	template<typename key_type, typename AllocatorT = global_allocator>
	struct _RB_tree 
	{
		using RB_Node = _RB_Node<key_type, AllocatorT>;
		using iterator = sequential_iterator<RB_Node>;
		using const_iterator = sequential_iterator<const RB_Node>;
		using data_type =key_type;

		RB_Node* _root;
		usize _count;
		AllocatorT _allocator;


		_RB_tree(const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator), _root(nullptr)
		{}

		//associative_array(std::initializer_list<key_type, value_type> list, const AllocatorT& allocator = AllocatorT())
		//	:_count(list.size()), _allocator(allocator)
		//{
		//	auto it = list.begin();
		//	for (usize i = 0; i < _count; ++i)
		//	{
		//		insert(it[0], it[1]);
		//		it = std::next(it);
		//	}
		//}

		//associative_array(const associative_array<key_type, value_type, AllocatorT>& other)
		//	:_count(other._count), _allocator(other._allocator)
		//{
		//	//_data_block = _allocator.alloc(other._data_block.size);

		//	for (usize i = 0; i < _count; ++i)
		//		new (_data_block.template at<T>(i)) T(*other._data_block.template at<T>(i));
		//}

		//dynamic_array(const dynamic_array<T, AllocatorT>& other, const AllocatorT& allocator)
		//	:_count(other._count),
		//	_allocator(allocator)
		//{
		//	_data_block = _allocator.alloc(other._data_block.size);

		//	for (usize i = 0; i < _count; ++i)
		//		new (_data_block.template at<T>(i)) T(*other._data_block.template at<T>(i));
		//}

		//dynamic_array(dynamic_array<T, AllocatorT>&& other)
		//	:_count(other._count),
		//	_data_block(tmp::move(other._data_block)),
		//	_allocator(tmp::move(other._allocator))
		//{
		//	other._count = 0;
		//}

		//dynamic_array(dynamic_array<T, AllocatorT>&& other, const AllocatorT& allocator)
		//	:_count(other._count),
		//	_data_block(tmp::move(other._data_block)),
		//	_allocator(allocator)
		//{
		//	other._count = 0;
		//}

		//modify if exisits, and insert if not.
		iterator 
		insert(const key_type& key/*, const value_type& value*/)
		{
			//auto temp = _allocator.template alloc<int>();
			//new (temp) RB_Node(key,_allocator/*, value*/);
			/*auto temp = RB_Node::create_node(key);

			if (_root == nullptr)
			{
				_root = &*temp;
				_root->recolor();
				++_count;
				return iterator(*_root);
			}
			else
			{
				auto it(_root->insert(&*temp));
				_get_root_back();
				++_count;
				return *it;
			}*/
			return nullptr;
		}
		
		void 
		_get_root_back()
		{

			while (_root->parent != nullptr)
			{
				_root = _root->parent;
			}
		}

		//void remove(key_type key);
		//Linear_search
		//void remove(value_type value);

		const_iterator 
		lookup(const key_type& key) const
		{
			if (_root != nullptr)
				return _root->lookup(key);
			return iterator(_root);
		}

		iterator 
		lookup(const key_type& key)
		{
			/*value_type* val = nullptr;

			if (_root != nullptr)
			{
				*val = _root->lookup(key);
			}
			return val;*/
			if (_root != nullptr)
				return _root->lookup(key);
			return iterator(_root);
		}

		//This function is for redblack tree
		iterator
		operator[](const key_type& key)
		{	//this function ether returns the existed node or the new inserted node
			return _root->insert(key);
		}

		////This function will be applied in associative array
		//value_type&
		//operator[](const key_type& key)
		//{
		//		auto it = _RB_tree[key];
		//		return it->value;
		////	auto it = _root->insert(key);
		////	if (it._element == nullptr)
		////	{
		////		it._element = _allocator.template alloc<RB_Node>();
		////		new (&(*it)) RB_Node(key);
		////	}
		////	return it->value;
		//}

		//value_type&
		//operator[](key_type&& key)
		//{
		//	//return _root->insert(key);
		//	auto it = _root->insert(tmp::move(key));
		//	if (it._element == nullptr)
		//	{
		//		it._element = _allocator.template alloc<RB_Node>();
		//		new (&(*it)) RB_Node(tmp::move(key));
		//	}
		//	return it->value;
		//}
		template<typename function_type>
		void 
		inorder_traverse(function_type FT) const
		{
			_root->inorder_traverse(FT);
		}

		usize
		count()
		{
			return _count;
		}


	};
}
