#include "cpprelude/platform.h"
#include "cpprelude/string.h"
#include "cpprelude/io.h"
#include "cpprelude/fmt.h"
#include <algorithm>

#if defined(OS_WINDOWS)
#include <Windows.h>
#include <Psapi.h>
#include <DbgHelp.h>
#undef min
#undef max
#elif defined(OS_LINUX)
#define _LARGEFILE64_SOURCE 1
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <execinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <cxxabi.h>
#endif

namespace cpprelude
{
	//platform
	platform_t::~platform_t()
	{
#ifdef DEBUG
		print_memory_report();
		//cleanup the configured debug mode stuff
		if(debug_configured)
		{
			#if defined(OS_WINDOWS)
			{
				auto result = SymCleanup(GetCurrentProcess());
				if(result)
					debug_configured = false;
			}
			#endif
		}
#endif
	}

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
		println_err("allocation count = ", allocation_count, "\n",
			"allocation size = ", allocation_size);
#endif
	}

	void
	platform_t::dump_callstack() const
	{
		#ifdef DEBUG

		constexpr usize MAX_NAME_LEN = 1024;
		constexpr usize STACK_MAX = 2048;
		void* callstack[STACK_MAX];

		#if defined(OS_WINDOWS)
		{
			auto process_handle = GetCurrentProcess();

			//allocate a buffer for the symbol info
			//windows lays the symbol info in memory in this form
			//[struct][name buffer]
			//and the name buffer size is the same as the MaxNameLen set below
			byte buffer[sizeof(SYMBOL_INFO) + MAX_NAME_LEN];

			SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(buffer);
			memset(symbol, 0, sizeof(SYMBOL_INFO));

			symbol->MaxNameLen = MAX_NAME_LEN;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			
			usize frames_count = CaptureStackBackTrace(0, STACK_MAX, callstack, NULL);
			for (usize i = 0; i < frames_count; ++i)
			{
				if (SymFromAddr(process_handle, (DWORD64)(callstack[i]), NULL, symbol))
					println_err("[", frames_count - i - 1, "]: ", symbol->Name);
				else
					println_err("[", frames_count - i - 1, "]: ", "unknown symbol");
			}
		}
		#elif defined(OS_LINUX)
		{
			//+1 for null terminated string
			char name_buffer[MAX_NAME_LEN+1];
			char demangled_buffer[MAX_NAME_LEN];
			usize demangled_buffer_length = MAX_NAME_LEN;

			//capture the call stack
			usize frames_count = backtrace(callstack, STACK_MAX);
			//resolve the symbols
			char** symbols = backtrace_symbols(callstack, frames_count);

			if(symbols)
			{
				for(usize i = 0; i < frames_count; ++i)
				{
					//isolate the function name
					char *name_begin = nullptr, *name_end = nullptr, *name_it = symbols[i];
					while(*name_it != 0)
					{
						if(*name_it == '(')
							name_begin = name_it+1;
						else if(*name_it == ')' || *name_it == '+')
						{
							name_end = name_it;
							break;
						}
						++name_it;
					}

					
					usize mangled_name_size = name_end - name_begin;
					//function maybe inlined
					if(mangled_name_size == 0)
					{
						println_err("[", frames_count - i - 1, "]: unknown/inlined symbol");
						continue;
					}

					//copy the function name into the name buffer
					usize copy_size = mangled_name_size > MAX_NAME_LEN ? MAX_NAME_LEN : mangled_name_size;
					memcpy(name_buffer, name_begin, copy_size);
					name_buffer[copy_size] = 0;

					int status = 0;
					abi::__cxa_demangle(name_buffer, demangled_buffer, &demangled_buffer_length, &status);
					if(status == 0)
					{
						auto function_name = string(make_slice(demangled_buffer, demangled_buffer_length), nullptr);
						println_err("[", frames_count - i - 1, "]: ", function_name);
					}
					else
					{
						auto function_name = string(make_slice(name_buffer, copy_size), nullptr);
						println_err("[", frames_count - i - 1, "]: ", function_name);
					}
				}
				::free(symbols);
			}
		}
		#endif

		#endif
	}

	result<file_handle, PLATFORM_ERROR>
	platform_t::file_open(const string& filename, IO_MODE io_mode, OPEN_MODE open_mode)
	{
		file_handle handle;
		#if defined(OS_WINDOWS)
		{
			//translate the io mode
			DWORD desired_access;
			switch(io_mode)
			{
				case IO_MODE::READ:
					desired_access = GENERIC_READ;
					break;

				case IO_MODE::WRITE:
					desired_access = GENERIC_WRITE;
					break;

				case IO_MODE::READ_WRITE:
				default:
					desired_access = GENERIC_READ | GENERIC_WRITE;
					break;
			}

			//translate the open mode
			DWORD creation_disposition;
			switch(open_mode)
			{
				case OPEN_MODE::CREATE_ONLY:
					creation_disposition = CREATE_NEW;
					break;

				case OPEN_MODE::OPEN_OVERWRITE:
					creation_disposition = TRUNCATE_EXISTING;
					break;

				case OPEN_MODE::OPEN_ONLY:
				case OPEN_MODE::OPEN_APPEND:
					creation_disposition = OPEN_EXISTING;
					break;

				case OPEN_MODE::CREATE_APPEND:
					creation_disposition = OPEN_ALWAYS;
					break;

				case OPEN_MODE::CREATE_OVERWRITE:
				default:
					creation_disposition = CREATE_ALWAYS;
					break;
			}

			//WinAPI doesn't play nice with utf-8 strings so i have to convert to utf-16 string
			constexpr i32 buffer_size = KILOBYTES(2);
			WCHAR utf16_buffer[buffer_size];
			auto size_needed = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, filename.data(), filename.size(), NULL, 0);
			LPWSTR win_filename;
			//i use small buffer to optimise for the common cases
			if (size_needed > buffer_size)
				win_filename = this->template alloc<WCHAR>(size_needed);
			else
				win_filename = utf16_buffer;
			MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, filename.data(), filename.size(), win_filename, size_needed);

			handle._win_handle = CreateFile(win_filename,
				desired_access,
				0,
				NULL,
				creation_disposition,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (size_needed > buffer_size)
				this->free(make_slice(win_filename, size_needed));
			
			if (handle._win_handle == INVALID_HANDLE_VALUE)
			{
				PLATFORM_ERROR err;
				switch(GetLastError())
				{
					case ERROR_FILE_EXISTS:
						err = PLATFORM_ERROR::FILE_ALREADY_EXISTS;
						break;
					case ERROR_FILE_NOT_FOUND:
						err = PLATFORM_ERROR::FILE_DOESNOT_EXIST;
						break;
					default:
						err = PLATFORM_ERROR::GENERIC_ERROR;
						break;
				}
				return err;
			}

			//move the cursor to the end of the file
			if (open_mode == OPEN_MODE::CREATE_APPEND ||
				open_mode == OPEN_MODE::OPEN_APPEND)
			{
				SetFilePointer (handle._win_handle,	//file handle
								NULL,				//distance to move low part
				 				NULL,				//ditance to mvoe high part
				 				FILE_END); 			//movement point of reference
			}
		}
		#elif defined(OS_LINUX)
		{
			int flags = 0;

			//translate the io mode
			switch(io_mode)
			{
				case IO_MODE::READ:
					flags |= O_RDONLY;
					break;

				case IO_MODE::WRITE:
					flags |= O_WRONLY;
					break;

				case IO_MODE::READ_WRITE:
				default:
					flags |= O_RDWR;
					break;
			}

			//translate the open mode
			switch(open_mode)
			{
				case OPEN_MODE::CREATE_ONLY:
					flags |= O_CREAT;
					flags |= O_EXCL;
					break;

				case OPEN_MODE::CREATE_APPEND:
					flags |= O_CREAT;
					flags |= O_APPEND;
					break;

				case OPEN_MODE::OPEN_ONLY:
					//do nothing
					break;

				case OPEN_MODE::OPEN_OVERWRITE:
					flags |= O_TRUNC;
					break;

				case OPEN_MODE::OPEN_APPEND:
					flags |= O_APPEND;
					break;

				case OPEN_MODE::CREATE_OVERWRITE:
				default:
					flags |= O_CREAT;
					flags |= O_TRUNC;
					break;
			}

			handle._linux_handle = open(filename.data(), flags);

			if(handle._linux_handle == -1)
			{
				if(errno == EEXIST)
					return PLATFORM_ERROR::FILE_ALREADY_EXISTS;
				else if(errno == ENOENT)
					return PLATFORM_ERROR::FILE_DOESNOT_EXIST;
				else
					return PLATFORM_ERROR::GENERIC_ERROR;
			}
		}
		#endif
		return handle;
	}

	bool
	platform_t::file_valid(const file_handle& handle)
	{
		#if defined(OS_WINDOWS)
			return handle._win_handle != nullptr;
		#elif defined(OS_LINUX)
			return handle._linux_handle != -1;
		#endif
	}

	bool
	platform_t::file_close(file_handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			auto result = CloseHandle(handle._win_handle);
			if(result)
				handle._win_handle = nullptr;
			return result;
		}
		#elif defined(OS_LINUX)
		{
			auto result = close(handle._linux_handle) == 0;
			if(result)
				handle._linux_handle = -1;
			return result;
		}
		#endif
	}

	bool
	platform_t::file_close(file_handle&& handle)
	{
		return file_close(handle);
	}

	usize
	platform_t::file_write(const file_handle& handle, const slice<byte>& data)
	{
		#if defined(OS_WINDOWS)
		{
			DWORD bytes_written = 0;
			WriteFile(handle._win_handle, data.ptr, data.size, &bytes_written, NULL);
			return bytes_written;
		}
		#elif defined(OS_LINUX)
		{
			return write(handle._linux_handle, data.ptr, data.size);
		}
		#endif
	}

	usize
	platform_t::file_read(const file_handle& handle, slice<byte>& data)
	{
		#if defined(OS_WINDOWS)
		{
			DWORD bytes_read = 0;
			ReadFile(handle._win_handle, data.ptr, data.size, &bytes_read, NULL);
			return bytes_read;
		}
		#elif defined(OS_LINUX)
		{
			return read(handle._linux_handle, data.ptr, data.size);
		}
		#endif
	}

	usize
	platform_t::file_read(const file_handle& handle, slice<byte>&& data)
	{
		return file_read(handle, data);
	}

	i64
	platform_t::file_size(const file_handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER size;
			if(GetFileSizeEx(handle._win_handle, &size))
			{
				return *reinterpret_cast<i64*>(&size);
			}
			return -1;
		}
		#elif defined(OS_LINUX)
		{
			struct stat file_stats;
			if(fstat(handle._linux_handle, &file_stats) == 0)
			{
				return file_stats.st_size;
			}
			return -1;
		}
		#endif
	}

	i64
	platform_t::file_cursor(const file_handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = 0;
			if(SetFilePointerEx(handle._win_handle, offset, &position, FILE_CURRENT))
			{
				return *reinterpret_cast<i64*>(&position);
			}
			return -1;
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = 0;
			return lseek64(handle._linux_handle, offset, SEEK_CUR);
		}
		#endif
	}

	bool
	platform_t::file_move(const file_handle& handle, i64 move_offset)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = move_offset;
			return SetFilePointerEx(handle._win_handle, offset, &position, FILE_CURRENT);
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = move_offset;
			return lseek64(handle._linux_handle, offset, SEEK_CUR) != -1;
		}
		#endif
	}

	bool
	platform_t::file_move_to_start(const file_handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = 0;
			return SetFilePointerEx(handle._win_handle, offset, &position, FILE_BEGIN);
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = 0;
			return lseek64(handle._linux_handle, offset, SEEK_SET) != -1;
		}
		#endif
	}

	bool
	platform_t::file_move_to_end(const file_handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = 0;
			return SetFilePointerEx(handle._win_handle, offset, &position, FILE_END);
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = 0;
			return lseek64(handle._linux_handle, offset, SEEK_END) != -1;
		}
		#endif
	}

	//private functions
	slice<byte>
	_default_alloc(void*, usize count)
	{
		using T = byte;
		if (count == 0)
			return slice<T>();
		T* ptr = reinterpret_cast<T*>(std::malloc(count * sizeof(T)));

#ifdef DEBUG
		++platform->allocation_count;
		platform->allocation_size += count * sizeof(T);
#endif

		return slice<T>(ptr, ptr ? count * sizeof(T) : 0);
	}

	void
	_default_free(void*, slice<byte>& slice_)
	{
		if (slice_.ptr != nullptr)
		{
#ifdef DEBUG
			--platform->allocation_count;
			platform->allocation_size -= slice_.size;
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
		platform->allocation_size += (count * sizeof(T)) - slice_.size;
		if (!slice_.valid()) ++platform->allocation_count;
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

	platform_t*
	_actual_init_platform()
	{
		//declare platform stuff
		static memory_context _global_memory;
		static platform_t _platform;
		static bool _is_initialized = false;

		if(_is_initialized)
			return &_platform;

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

			//configure the debug mode stuff
			#ifdef DEBUG
			{
				auto result = SymInitialize(GetCurrentProcess(), NULL, true);
				if(result)
					_platform.debug_configured = true;
			}
			#endif
		#endif

		_is_initialized = true;
		//return the created platform
		return &_platform;
	}

	platform_t* platform = _actual_init_platform();

	void
	_init_platform()
	{
		platform = _actual_init_platform();
	}
}