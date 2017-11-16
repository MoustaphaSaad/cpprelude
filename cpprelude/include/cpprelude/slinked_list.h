#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/iterator.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/platform.h"
#include <initializer_list>
#include <iterator>

namespace cpprelude
{
	/*
	 * data is layouted this way
	 * owner_mem_block = [ sizeof(owner_mem_block) | T ]
	 * this has two parts a next owner_mem_block which has the next element
	 * and a T which houses the data
	 */
	template<typename T>
	struct slinked_list
	{
		using iterator = forward_iterator<T>;
		using const_iterator = const_forward_iterator<T>;
		using data_type = T;
		using node_type = details::single_node<T>;

		node_type* _head;
		usize _count;
		memory_context *_context = platform.global_memory;

		slinked_list(memory_context* context = platform.global_memory)
			:_head(nullptr), _count(0), _context(context)
		{}

		slinked_list(std::initializer_list<T> list, memory_context* context = platform.global_memory)
			:_head(nullptr), _count(0), _context(context)
		{
			auto it = list.end();
			it = std::prev(it);
			for(usize i = 0; i < list.size(); ++i)
			{
				insert_front(*it);
				it = std::prev(it);
			}
		}

		slinked_list(usize count, const T& fill_value, memory_context* context = platform.global_memory)
			:_head(nullptr), _count(0), _context(context)
		{
			for(usize i = 0; i < count; ++i)
				insert_front(fill_value);
		}

		slinked_list(const slinked_list<T>& other)
			:_head(nullptr), _count(0), _context(other._context)
		{
			auto* other_it = &other._head;
			auto* it = &_head;

			for(usize i = 0;
				i < other._count;
				++i)
			{
				*it = _context->template alloc<node_type>();

				//copy the data
				new (&(*it)->data) T((*other_it)->data);

				it = &(*it)->next;
				other_it = &(*other_it)->next;
				++_count;
			}
		}

		slinked_list(const slinked_list<T>& other, memory_context* context)
			:_head(nullptr), _count(0), _context(context)
		{
			auto* other_it = &other._head;
			auto* it = &_head;

			for(usize i = 0;
				i < other._count;
				++i)
			{
				*it = _context->template alloc<node_type>();

				//copy the data
				new (&(*it)->data) T((*other_it)->data);

				it = &(*it)->next;
				other_it = &(*other_it)->next;
				++_count;
			}
		}

		slinked_list(slinked_list<T>&& other)
			:_head(other._head),
			 _count(other._count),
			 _context(other._context)
		{
			other._count = 0;
			other._head = nullptr;
			other._context = nullptr;
		}

		slinked_list(slinked_list<T>&& other, memory_context* context)
			:_head(other._head),
			 _count(other._count),
			 _context(context)
		{
			other._count = 0;
			other._head = nullptr;
			other._context = nullptr;
		}

		~slinked_list()
		{
			reset();
		}

		slinked_list<T>&
		operator=(const slinked_list<T>& other)
		{
			reset();

			_context = other._context;

			auto* other_it = &other._head;
			auto* it = &_head;

			for(usize i = 0;
				i < other._count;
				++i)
			{
				*it = _context->template alloc<node_type>();

				//copy the data
				new (&(*it)->data) T((*other_it)->data);

				it = &(*it)->next;
				other_it = &(*other_it)->next;
				++_count;
			}

			return *this;
		}

		slinked_list<T>&
		operator=(slinked_list<T>&& other)
		{
			reset();
			_count = other._count;
			_head = other._head;
			_context = std::move(other._context);
			other._count = 0;
			other._head = nullptr;

			return *this;
		}

		usize
		count() const
		{
			return _count;
		}

		void
		expand_front(usize additional_count, const T& fill_value)
		{
			auto new_count = _count + additional_count;
			for(usize i = _count; i < new_count; ++i)
				insert_front(fill_value);
		}

		void
		shrink_front(usize shrinkage_count)
		{
			remove_front(shrinkage_count);
		}

		T&
		operator[](usize index)
		{
			auto it = _head;
			while(index--)
			{
				it = it->next;
			}
			return it->data;
		}

		const T&
		operator[](usize index) const
		{
			auto it = _head;
			while(index--)
			{
				it = it->next;
			}
			return it->data;
		}

		void
		insert_front(std::initializer_list<T> list)
		{
			auto it = list.end();
			it = std::prev(it);
			for(usize i = 0; i < list.size(); ++i)
			{
				insert_front(*it);
				it = std::prev(it);
			}
		}

		template<typename ... TArgs>
		void
		emplace_front(TArgs&& ... args)
		{
			node_type* new_node = _context->template alloc<node_type>();

			//copy the data value
			new (&new_node->data) T(std::forward<TArgs>(args)...);

			//move the current head as the next to the new node
			new_node->next = _head;

			//set the new node as the new head
			_head = new_node;

			++_count;
		}

		void
		insert_front(const T& value)
		{
			node_type* new_node = _context->template alloc<node_type>();

			//copy the data value
			new (&new_node->data) T(value);

			//move the current head as the next to the new node
			new_node->next = _head;

			//set the new node as the new head
			_head = new_node;

			++_count;
		}

		void
		insert_front(T&& value)
		{
			node_type* new_node = _context->template alloc<node_type>();

			//copy the data value
			new (&new_node->data) T(std::move(value));

			//move the current head as the next to the new node
			new_node->next = _head;

			//set the new node as the new head
			_head = new_node;

			++_count;
		}

		template<typename ... TArgs>
		void
		emplace_after(const iterator& it, TArgs&& ... args)
		{
			node_type* new_node = _context->template alloc<node_type>();

			//copy the data value
			new (&new_node->data) T(std::forward<TArgs>(args)...);

			//set the next of new node to the next of the sent iterator
			new_node->next = it._node->next;
			//set the new_node as the next of the sent iterator
			it._node->next = new_node;

			++_count;
		}

		void
		insert_after(const iterator& it, const T& value)
		{
			node_type* new_node = _context->template alloc<node_type>();

			//copy the data value
			new (&new_node->data) T(value);

			//set the next of new node to the next of the sent iterator
			new_node->next = it._node->next;
			//set the new_node as the next of the sent iterator
			it._node->next = new_node;

			++_count;
		}

		void
		insert_after(const iterator& it, T&& value)
		{
			node_type* new_node = _context->template alloc<node_type>();

			//copy the data value
			new (&new_node->data) T(std::move(value));

			//set the next of new node to the next of the sent iterator
			new_node->next = it._node->next;
			//set the new_node as the next of the sent iterator
			it._node->next = new_node;

			++_count;
		}

		void
		remove_front(usize removal_count = 1)
		{
			if(removal_count == 0)
				return;

			auto it = _head;
			for(usize i = 0;
				i < removal_count;
				++i)
			{
				//move the next node to the tmp
				auto next_node = it->next;

				//destroy the value
				it->data.~T();

				//free the memory
				if (_context)
					_context->free(make_slice(it));

				it = next_node;
				--_count;

			}
			_head = it;
		}

		void
		reset()
		{
			auto it = _head;
			while(_count)
			{
				//move the next node to the tmp
				auto next_node = it->next;

				//destroy the value
				it->data.~T();

				//free the memory
				if(_context)
					_context->free(make_slice(it));

				it = next_node;
				--_count;
			}
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		const_iterator
		front() const
		{
			return const_iterator(_head);
		}

		iterator
		front()
		{
			return iterator(_head);
		}

		const_iterator
		cbegin() const
		{
			return const_iterator(_head);
		}

		const_iterator
		begin() const
		{
			return const_iterator(_head);
		}

		iterator
		begin()
		{
			return iterator(_head);
		}

		const_iterator
		cend() const
		{
			return const_iterator();
		}

		const_iterator
		end() const
		{
			return const_iterator();
		}

		iterator
		end()
		{
			return iterator();
		}

		slice<node_type>
		decay()
		{
			slice<node_type> result(_head, sizeof(node_type));
			_head = nullptr;
			_count = 0;
			return result;
		}

		slice<T>
		decay_continuous()
		{
			//allocate the memory of the supplied allocator
			slice<T> result = _context->template alloc<T>(_count);

			//move the elements over
			usize i = 0;
			for(auto&& value: *this)
				new (&result[i++]) T(std::move(value));

			//reset this linked list
			reset();

			//return the piece of memory
			return result;
		}
	};
}
