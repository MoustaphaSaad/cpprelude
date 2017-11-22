#include "cpprelude/platform.h"
#include <mutex>
#include <algorithm>
#include <iostream>

#if defined(OS_WINDOWS)
#include <Windows.h>
#include <Psapi.h>
#undef min
#undef max
#elif defined(OS_LINUX)
#include <sys/mman.h>
#include <sys/sysinfo.h>
#endif

namespace cpprelude
{
	//platform
	platform_t::~platform_t()
	{
#ifdef DEBUG
		print_memory_report();
#endif
	}

	slice<byte>
		platform_t::virtual_alloc(void* address_hint, usize size)
	{
		if (size == 0)
			return slice<byte>();

		void* result = nullptr;

#if defined(OS_WINDOWS)
		result = VirtualAlloc(address_hint, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#elif defined(OS_LINUX)
		result = mmap(address_hint, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif

		return make_slice(reinterpret_cast<byte*>(result), size);
	}

	bool
		platform_t::virtual_free(slice<byte>& data)
	{
#if defined(OS_WINDOWS)
		return VirtualFree(data.ptr, 0, MEM_RELEASE) != NULL;
#elif defined(OS_LINUX)
		return munmap(data.ptr, data.size) == 0;
#endif
	}

	bool
		platform_t::virtual_free(slice<byte>&& data)
	{
		return virtual_free(data);
	}

	void
		platform_t::print_memory_report() const
	{
#ifdef DEBUG
		println(std::cout,
			"allocation count = ", allocation_count, "\n",
			"allocation size = ", allocation_size);
#endif
	}

	slice<byte>
		_default_alloc(void*, usize count)
	{
		using T = byte;
		if (count == 0)
			return slice<T>();
		T* ptr = reinterpret_cast<T*>(std::malloc(count * sizeof(T)));

#ifdef DEBUG
		++platform.allocation_count;
		platform.allocation_size += count * sizeof(T);
#endif

		return slice<T>(ptr, ptr ? count * sizeof(T) : 0);
	}

	void
		_default_free(void*, slice<byte>& slice_)
	{
		if (slice_.ptr != nullptr)
		{
#ifdef DEBUG
			--platform.allocation_count;
			platform.allocation_size -= slice_.size;
#endif
			std::free(slice_.ptr);
		}

		slice_.ptr = nullptr;
		slice_.size = 0;
	}

	void
		_default_realloc(void* self, slice<byte>& slice_, usize count)
	{
		using T = byte;
		if (count == 0)
		{
			_default_free(self, slice_);
			return;
		}

#ifdef DEBUG
		platform.allocation_size += (count * sizeof(T)) - slice_.size;
		if (!slice_.valid()) ++platform.allocation_count;
#endif

		slice_.ptr = reinterpret_cast<T*>(std::realloc(slice_.ptr, count * sizeof(T)));
		slice_.size = count * sizeof(T);
	}

	usize
		_get_ram_size()
	{
		usize totalram = 0;
#if defined(OS_LINUX)
		{
			struct sysinfo info;
			sysinfo(&info);
			totalram = std::max<usize>(GIGABYTES(4) - 1ULL, (info.totalram * 1024ULL));
		}
#elif defined(OS_WINDOWS)
		{
			ULONGLONG result = 0; //size of physical memory in kilobytes
			GetPhysicallyInstalledSystemMemory(&result);
			totalram = std::max<usize>(GIGABYTES(4) - 1ULL, (result * 1024ULL));
		}
#endif
		return totalram;
	}

	platform_t&
		_init_platform()
	{
		//declare platform stuff
		static memory_context _global_memory;
		static platform_t _platform;

		//setup the memory
		_global_memory._self = &_platform;
		_global_memory._alloc = _default_alloc;
		_global_memory._realloc = _default_realloc;
		_global_memory._free = _default_free;

		//setup the platform
		_platform.global_memory = &_global_memory;
		_platform.allocation_count = 0;
		_platform.allocation_size = 0;
		_platform.RAM_SIZE = _get_ram_size();

		//windows setup stuff
#if defined(OS_WINDOWS)
		//set the console mode to support utf8
		SetConsoleOutputCP(CP_UTF8);
#endif

		//return the created platform
		return _platform;
	}

	platform_t& platform = _init_platform();

	//print stuff
	namespace helper
	{
		inline static std::mutex&
			print_lock()
		{
			static std::mutex _print_lock;
			return _print_lock;
		}

		void
			__acquire_print_lock()
		{
			print_lock().lock();
		}

		void
			__release_print_lock()
		{
			print_lock().unlock();
		}
	}
}