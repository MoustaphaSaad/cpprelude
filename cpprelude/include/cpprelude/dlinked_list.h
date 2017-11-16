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
	 * owner_mem_block = [sizeof(owner_mem_block): "prev" | sizeof(owner_mem_block): "next" | T]
	 * this has three parts a previous owner_mem_block which has the previous element, a next owner_mem_block which has the next element and a T which houses the data
	 */

	template<typename T>
	struct dlinked_list
	{
		using iterator = bidirectional_iterator<T>;
		using const_iterator = const_bidirectional_iterator<T>;
		using data_type = T;
		using node_type = details::double_node<T>;

		node_type *_head, *_tail;
		usize _count;
		memory_context *_context = platform.global_memory;

		dlinked_list(memory_context* context = platform.global_memory)
			:_head(nullptr), _tail(nullptr), _count(0), _context(context)
		{
			_init_sentinels();
		}

		dlinked_list(std::initializer_list<T> list, memory_context* context = platform.global_memory)
			:_head(nullptr), _tail(nullptr), _count(0), _context(context)
		{
			_init_sentinels();

			for(auto& value: list)
				insert_back(value);
		}

		dlinked_list(usize count, const T& fill_value, memory_context* context = platform.global_memory)
			:_head(nullptr), _tail(nullptr), _count(0), _context(context)
		{
			_init_sentinels();

			for(usize i = 0; i < count; ++i)
				insert_back(fill_value);
		}

		dlinked_list(const dlinked_list<T>& other)
			:_head(nullptr), _tail(nullptr), _count(0), _context(other._context)
		{
			_init_sentinels();

			for(const auto& other_value: other)
				insert_back(other_value);
		}

		dlinked_list(const dlinked_list<T>& other, memory_context* context)
			:_head(nullptr), _tail(nullptr), _count(0), _context(context)
		{
			_init_sentinels();

			for(auto& other_value: other)
				insert_back(other_value);
		}

		dlinked_list(dlinked_list<T>&& other)
			:_head(other._head),
			 _tail(other._tail),
			 _count(other._count),
			_context(other._context)
		{
			other._count = 0;
			other._head = nullptr;
			other._tail = nullptr;
			other._context = nullptr;
		}

		dlinked_list(dlinked_list<T>&& other, memory_context* context)
			:_head(other._head),
			 _tail(other._tail),
			 _count(other._count),
			 _context(context)
		{
			other._count = 0;
			other._head = nullptr;
			other._tail = nullptr;
			other._context = nullptr;
		}

		~dlinked_list()
		{
			reset();
			//remove the sentinals
			if(_context)
			{
				_context->free(make_slice(_head));
				_context->free(make_slice(_tail));
			}
			_count = 0;
			_context = nullptr;
		}

		dlinked_list<T>&
		operator=(const dlinked_list<T>& other)
		{
			reset();
			_context = other._context;

			for(auto& other_value: other)
				insert_back(other_value);

			return *this;
		}

		dlinked_list<T>&
		operator=(dlinked_list<T>&& other)
		{
			reset();
			//free the sentinals as we are going to steal them
			_context->free(make_slice(_head));
			_context->free(make_slice(_tail));
			_context = other._context;

			_count = other._count;
			_head = other._head;
			_tail = other._tail;
			other._count = 0;
			other._head = nullptr;
			other._tail = nullptr;
			other._context = nullptr;

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
		expand_back(usize additional_count, const T& fill_value)
		{
			auto new_count = _count + additional_count;
			for(usize i = _count; i < new_count; ++i)
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
			if(index >= _count * 0.5)
			{
				index = _count - index;
				auto it = _tail;
				while(index--)
					it = it->prev;

				return it->data;
			}
			else
			{
				auto it = _head->next;
				while(index--)
					it = it->next;

				return it->data;
			}
		}

		const T&
		operator[](usize index) const
		{
			if(index >= _count * 0.5)
			{
				index = _count - index;
				auto it = _tail;
				while(index--)
					it = it->prev;

				return it->data;
			}
			else
			{
				auto it = _head->next;
				while(index--)
					it = it->next;

				return it->data;
			}
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
			new (&new_node->data) T(std::forward<TArgs>(args)...);

			_insert_front_helper(_head->next, new_node);

			++_count;
		}

		void
		insert_front(const T& value)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(value);

			_insert_front_helper(_head->next, new_node);

			++_count;
		}

		void
		insert_front(T&& value)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(std::move(value));

			_insert_front_helper(_head->next, new_node);

			++_count;
		}

		void
		insert_back(std::initializer_list<T> list)
		{
			for(const auto& value: list)
				insert_back(value);
		}

		template<typename ... TArgs>
		void
		emplace_back(TArgs&& ... args)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(std::forward<TArgs>(args)...);

			_insert_back_helper(_tail->prev, new_node);

			++_count;
		}

		void
		insert_back(const T& value)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(value);

			_insert_back_helper(_tail->prev, new_node);

			++_count;
		}

		void
		insert_back(T&& value)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(std::move(value));

			_insert_back_helper(_tail->prev, new_node);

			++_count;
		}

		template<typename ... TArgs>
		void
		emplace_after(const iterator& it, TArgs&& ... args)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(std::forward<TArgs>(args)...);

			_insert_back_helper(it._node, new_node);

			++_count;
		}

		void
		insert_after(const iterator& it, const T& value)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(value);

			_insert_back_helper(it._node, new_node);

			++_count;
		}

		void
		insert_after(const iterator& it, T&& value)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(std::move(value));

			_insert_back_helper(it._node, new_node);

			++_count;
		}

		template<typename ... TArgs>
		void
		emplace_before(const iterator& it, TArgs&& ... args)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(std::forward<TArgs>(args)...);

			_insert_front_helper(it._node, new_node);

			++_count;
		}

		void
		insert_before(const iterator& it, const T& value)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(value);

			_insert_front_helper(it._node, new_node);

			++_count;
		}

		void
		insert_before(const iterator& it, T&& value)
		{
			node_type* new_node = _context->template alloc<node_type>();
			new (&new_node->data) T(std::move(value));

			_insert_front_helper(it._node, new_node);

			++_count;
		}

		void
		remove_front(usize removal_count = 1)
		{
			if(removal_count == 0)
				return;

			auto it = _head->next;

			while(removal_count--)
			{
				auto next_node = it->next;

				it->data.~T();

				_context->free(make_slice(it));

				it = next_node;
				--_count;
			}

			it->prev = _head;
			_head->next = it;
		}

		void
		remove_back(usize removal_count = 1)
		{
			auto it = _tail->prev;

			while(removal_count--)
			{
				auto prev_node = it->prev;

				it->data.~T();

				_context->free(make_slice(it));

				it = prev_node;
				--_count;
			}

			it->next = _tail;
			_tail->prev = it;
		}

		void remove(iterator it)
		{
			auto node = it._node;
			auto prev_node = node->prev;
			auto next_node = node->next;

			prev_node->next = next_node;
			next_node->prev = prev_node;

			node->data.~T();
			_context->free(make_slice(node));
			--_count;
		}

		void
		reset()
		{
			if(_count == 0)
				return;

			auto it = _head->next;

			while(_count)
			{
				auto next_node = it->next;

				it->data.~T();

				if(_context) _context->free(make_slice(it));

				it = next_node;

				--_count;
			}

			_head->next = _tail;
			_tail->prev = _head;
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		const_iterator
		front() const
		{
			return const_iterator(_head->next);
		}

		iterator
		front()
		{
			return iterator(_head->next);
		}

		const_iterator
		back() const
		{
			return const_iterator(_tail->prev);
		}

		iterator
		back()
		{
			return iterator(_tail->prev);
		}

		const_iterator
		cbegin() const
		{
			return const_iterator(_head->next);
		}

		const_iterator
		begin() const
		{
			return const_iterator(_head->next);
		}

		iterator
		begin()
		{
			return iterator(_head->next);
		}

		const_iterator
		cend() const
		{
			return const_iterator(_tail);
		}

		const_iterator
		end() const
		{
			return const_iterator(_tail);
		}

		iterator
		end()
		{
			return iterator(_tail);
		}

		slice<node_type>
		decay()
		{
			slice<node_type> result(_head);

			_head = nullptr;
			_tail = nullptr;
			_count = 0;
			return result;
		}

		slice<T>
		decay_continuous()
		{
			//allocate the memory of supplied allocator
			auto result = _context->template alloc<T>(_count);

			//move the elements over
			usize i = 0;
			for(auto&& value: *this)
				new (&result[i++]) T(std::move(value));

			//reset this linked list
			reset();

			//return the piece of memory
			return result;
		}

		void
		_init_sentinels()
		{
			_head = _context->template alloc<node_type>();
			_tail = _context->template alloc<node_type>();

			_tail->prev = _head;
			_head->next = _tail;
			_head->prev = nullptr;
			_tail->next = nullptr;
		}

		inline void
		_insert_front_helper(node_type* old_node, node_type* new_node)
		{
			old_node->prev->next = new_node;
			new_node->prev = old_node->prev;

			old_node->prev = new_node;
			new_node->next = old_node;
		}

		inline void
		_insert_back_helper(node_type* old_node, node_type* new_node)
		{
			old_node->next->prev = new_node;
			new_node->next = old_node->next;

			old_node->next = new_node;
			new_node->prev = old_node;
		}
	};
}
