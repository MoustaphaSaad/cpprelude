#include "cpprelude/platform.h"
#include <mutex>
#include <algorithm>

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
	slice<byte>
	platform_t::virtual_alloc(void* address_hint, usize size)
	{
		if(size == 0)
			return slice<byte>();

		void* result = nullptr;

		#if defined(OS_WINDOWS)
			result = VirtualAlloc(address_hint, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
		#elif defined(OS_LINUX)
			result = mmap(address_hint, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		#endif

		return make_slice(reinterpret_cast<byte*>(result), size);
	}

	bool
	platform_t::virtual_free(slice<byte>& data)
	{
		#if defined(OS_WINDOWS)
			return VirtualFree(data.ptr, data.size, MEM_RELEASE) != NULL;
		#elif defined(OS_LINUX)
			return munmap(data.ptr, data.size) == 0;
		#endif
	}

	bool
	platform_t::virtual_free(slice<byte>&& data)
	{
		return virtual_free(data);
	}

	slice<byte>
	_default_alloc(void*, usize size)
	{
		return alloc<byte>(size);
	}

	void
	_default_realloc(void*, slice<byte>& data, usize size)
	{
		realloc(data, size);
	}

	void
	_default_free(void*, slice<byte>& data)
	{
		free(data);
	}

	usize
	_get_ram_size()
	{
		usize totalram = 0;
		#if defined(OS_LINUX)
		{
			struct sysinfo info;
			sysinfo(&info);
			totalram = std::max<usize>(GIGABYTES(4) - 1ULL, (info.totalram * 1024ULL) - 1 ULL);
		}
		#elif defined(OS_WINDOWS)
		{
			ULONGLONG result = 0; //size of physical memory in kilobytes
			GetPhysicallyInstalledSystemMemory(&result);
			totalram = std::max<usize>(GIGABYTES(4) - 1ULL, (result * 1024ULL) - 1ULL);
		}
		#endif
		return totalram;
	}

	platform_t&
	_init_platform()
	{
		//declare platform stuff
		static memory_context_t _global_memory;
		static platform_t _platform;

		//setup the memory
		_global_memory._self = &_platform;
		_global_memory._alloc = _default_alloc;
		_global_memory._realloc = _default_realloc;
		_global_memory._free = _default_free;

		//setup the platform
		_platform.global_memory = &_global_memory;
		_platform.RAM_SIZE = _get_ram_size();

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