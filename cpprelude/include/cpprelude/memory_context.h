#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	struct memory_context
	{
		using alloc_func 		= slice<byte>(*)(void*, usize);
		using realloc_func		= void(*)(void*, slice<byte>&, usize);
		using free_func 		= void(*)(void*, slice<byte>&);

		void* _self = nullptr;
		alloc_func _alloc = nullptr;
		realloc_func _realloc = nullptr;
		free_func _free = nullptr;

		template<typename T>
		slice<T>
		alloc(usize count = 1)
		{
			return _alloc(_self, sizeof(T) * count).template convert<T>();
		}

		template<typename T>
		void
		free(slice<T>& data)
		{
			auto byte_block = data.template convert<byte>();
			_free(_self, byte_block);
			data = byte_block.template convert<T>();
		}

		template<typename T>
		void
		free(slice<T>&& data)
		{
			auto byte_block = data.template convert<byte>();
			_free(_self, byte_block);
			data = byte_block.template convert<T>();
		}

		template<typename T>
		void
		realloc(slice<T>& data, usize count)
		{
			auto byte_block = data.template convert<byte>();
			_realloc(_self, byte_block, count * sizeof(T));
			data = byte_block.template convert<T>();
		}

		template<typename T>
		void
		realloc(slice<T>&& data, usize count)
		{
			auto byte_block = data.template convert<byte>();
			_realloc(_self, byte_block, count * sizeof(T));
			data = byte_block.template convert<T>();
		}
	};
}