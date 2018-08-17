#include "cpprelude/OS.h"
#include "cpprelude/IO.h"
#include "cpprelude/Buffered_Stream.h"
#include "cpprelude/Dynamic_Array.h"
#include "sewing-fcontext/fcontext.h"
#include <mutex>
#include <stdlib.h>

#if defined(OS_WINDOWS)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
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

static_assert(sizeof(intptr_t) == sizeof(void*),
	"boost context argument size assumption failed, jump_fcontext will crash.");


namespace cppr
{
	Owner<byte>
	_malloc(void* _self, usize size)
	{
		return Owner<byte>((byte*)::malloc(size), size);
	}

	void
	_free(void* _self, const Owner<byte>& value)
	{
		::free(value.ptr);
	}
	
	struct Memory_Block
	{
		usize size = 0;
		Memory_Stream call_stack;
		Memory_Block *next = nullptr, *prev = nullptr;
	};

	static Allocator_Trait 	_mallocator { nullptr, _malloc, _free };
	static Memory_Block* 	_leak_head = nullptr;
	static std::mutex		_leak_mtx;

	OS::~OS()
	{
		#if defined(OS_WINDOWS) && defined(DEBUG)
		{
			if(debug_configured)
			{
				auto c = GetCurrentProcess();
				if(SymCleanup(c))
					debug_configured = false;
			}
		}
		#endif
		print_leak_report();
	}

	void
	OS::print_leak_report(IO_Trait* io) const
	{
		if(_leak_head)
		{
			if(io == nullptr) io = unbuf_stderr;
			usize count = 0;
			usize size = 0;
			auto it = _leak_head;
			while(it)
			{
				vprints(io, "Leak size: ", it->size, ", call stack:\n");
				if(it->call_stack.empty())
					vprints(io, "run in debug mode to get call stack info\n");
				else
					vprints(io, it->call_stack.str_content(), "\n");
				count++;
				size += it->size;
				it = it->next;
			}
			vprints(io, "Leaks count: ", count, ", Leaks size(bytes): ", size, "\n");
		}
	}

	void
	OS::dump_callstack(IO_Trait* io) const
	{
		#ifdef DEBUG
		{
			if(io == nullptr) io = unbuf_stderr;
			constexpr usize MAX_NAME_LEN = 1024;
			constexpr usize STACK_MAX = 4096;
			void* callstack[STACK_MAX];

			#if defined(OS_WINDOWS)
			{
				auto process_handle = GetCurrentProcess();

				//allocaet a buffer for the symbol info
				//windows lays the symbol info in memory in this form
				//[struct][name buffer]
				//and the name buffer size is the same as the MaxNameLen set below
				byte buffer[sizeof(SYMBOL_INFO) + MAX_NAME_LEN];

				SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(buffer);
				memset(symbol, 0, sizeof(SYMBOL_INFO));

				symbol->MaxNameLen = MAX_NAME_LEN;
				symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

				usize frames_count = CaptureStackBackTrace(0, STACK_MAX, callstack, NULL);
				for(usize i = 0; i < frames_count; ++i)
				{
					if(SymFromAddr(process_handle, (DWORD64)(callstack[i]), NULL, symbol))
						vprints(io, "[", frames_count - i - 1, "]: ", symbol->Name, "\n");
					else
						vprints(io, "[", frames_count - i - 1, "]: unknown symbol", "\n");
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
							vprints(io, "[", frames_count - i - 1, "]: unknown/inlined symbol\n");
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
							String_Range function_name = make_strrng(demangled_buffer, demangled_buffer_length);
							vprints(io, "[", frames_count - i - 1, "]: ", function_name, "\n");
						}
						else
						{
							String_Range function_name = make_strrng(name_buffer, copy_size);
							vprints(io, "[", frames_count - i - 1, "]: ", function_name, "\n");
						}
					}
					::free(symbols);
				}
			}
			#endif
		}
		#endif
	}

	Owner<byte>
	OS::virtual_alloc(void* address_hint, usize size)
	{
		if(size == 0)
			return Owner<byte>();

		void* result = nullptr;

		#if defined(OS_WINDOWS)
			result = VirtualAlloc(address_hint, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
		#elif defined(OS_LINUX)
			result = mmap(address_hint, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		#endif

		return own((byte*)result, size);
	}

	bool
	OS::virtual_free(const Owner<byte>& data)
	{
		#if defined(OS_WINDOWS)
			return VirtualFree(data.ptr, 0, MEM_RELEASE) != NULL;
		#elif defined(OS_LINUX)
			return munmap(data.ptr, data.size) == 0;
		#endif
	}

	//file stuff
	Result<File_Handle, OS_ERROR>
	OS::file_open(const String_Range& filename,
				  IO_MODE io_mode, OPEN_MODE open_mode)
	{
		File_Handle handle;
		OS_ERROR err = OS_ERROR::OK;
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
			auto size_needed = MultiByteToWideChar(CP_UTF8,
									MB_PRECOMPOSED, filename.data(), filename.size(), NULL, 0);
			//for zero termination
			++size_needed;

			LPWSTR win_filename;
			//i use small buffer to optimise for the common cases
			if (size_needed > buffer_size)
				win_filename = global_memory->template alloc<WCHAR>(size_needed).ptr;
			else
				win_filename = utf16_buffer;
			MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED,
					filename.data(), filename.size(), win_filename, size_needed);
			win_filename[size_needed - 1] = L'\0';

			handle.windows_handle = CreateFile(win_filename, desired_access, 0, NULL,
											   creation_disposition,
											   FILE_ATTRIBUTE_NORMAL,
											   NULL);

			if(size_needed > buffer_size)
				global_memory->free(own(win_filename, size_needed));

			if (handle.windows_handle == INVALID_HANDLE_VALUE)
			{
				switch(GetLastError())
				{
					case ERROR_FILE_EXISTS:
						err = OS_ERROR::FILE_ALREADY_EXISTS;
						break;
					case ERROR_FILE_NOT_FOUND:
						err = OS_ERROR::FILE_DOESNOT_EXIST;
						break;
					default:
						err = OS_ERROR::GENERIC_ERROR;
						break;
				}
				return {handle, err};
			}

			//move the cursor to the end of the file
			if (open_mode == OPEN_MODE::CREATE_APPEND ||
				open_mode == OPEN_MODE::OPEN_APPEND)
			{
				SetFilePointer (handle.windows_handle,	//file handle
								NULL,					//distance to move low part
				 				NULL,					//ditance to mvoe high part
				 				FILE_END); 				//movement point of reference
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

			handle.linux_handle = open(filename.data(), flags, S_IRWXU);

			if(handle.linux_handle == -1)
			{
				if(errno == EEXIST)
					err = OS_ERROR::FILE_ALREADY_EXISTS;
				else if(errno == ENOENT)
					err = OS_ERROR::FILE_DOESNOT_EXIST;
				else
					err = OS_ERROR::GENERIC_ERROR;
			}
		}
		#endif

		return {handle, err};
	}

	bool
	OS::file_close(File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			auto result = CloseHandle(handle.windows_handle);
			if(result)
				handle.windows_handle = nullptr;
			return result;
		}
		#elif defined(OS_LINUX)
		{
			auto result = close(handle.linux_handle) == 0;
			if(result)
				handle.linux_handle = -1;
			return result;
		}
		#endif
	}

	bool
	OS::file_close(File_Handle&& handle)
	{
		return file_close(handle);
	}

	bool
	OS::file_valid(const File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			return handle.windows_handle != nullptr;
		}
		#elif defined(OS_LINUX)
		{
			return handle.linux_handle != -1;
		}
		#endif
	}

	usize
	OS::file_write(const File_Handle& handle, const Slice<byte>& data)
	{
		#if defined(OS_WINDOWS)
		{
			DWORD bytes_written = 0;
			WriteFile(handle.windows_handle, data.ptr, data.size, &bytes_written, NULL);
			return bytes_written;
		}
		#elif defined(OS_LINUX)
		{
			return write(handle.linux_handle, data.ptr, data.size);
		}
		#endif
	}

	usize
	OS::file_read(const File_Handle& handle, Slice<byte>& data)
	{
		#if defined(OS_WINDOWS)
		{
			DWORD bytes_read = 0;
			ReadFile(handle.windows_handle, data.ptr, data.size, &bytes_read, NULL);
			return bytes_read;
		}
		#elif defined(OS_LINUX)
		{
			return ::read(handle.linux_handle, data.ptr, data.size);
		}
		#endif
	}

	usize
	OS::file_read(const File_Handle& handle, Slice<byte>&& data)
	{
		return this->file_read(handle, data);
	}

	i64
	OS::file_size(const File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER size;
			if(GetFileSizeEx(handle.windows_handle, &size))
			{
				return *reinterpret_cast<i64*>(&size);
			}
			return -1;
		}
		#elif defined(OS_LINUX)
		{
			struct stat file_stats;
			if(fstat(handle.linux_handle, &file_stats) == 0)
			{
				return file_stats.st_size;
			}
			return -1;
		}
		#endif
	}

	i64
	OS::file_cursor_position(const File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = 0;
			if(SetFilePointerEx(handle.windows_handle, offset, &position, FILE_CURRENT))
			{
				return *reinterpret_cast<i64*>(&position);
			}
			return -1;
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = 0;
			return lseek64(handle.linux_handle, offset, SEEK_CUR);
		}
		#endif
	}

	bool
	OS::file_cursor_move(const File_Handle& handle, i64 move_offset)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = move_offset;
			return SetFilePointerEx(handle.windows_handle, offset, &position, FILE_CURRENT);
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = move_offset;
			return lseek64(handle.linux_handle, offset, SEEK_CUR) != -1;
		}
		#endif
	}

	bool
	OS::file_cursor_move_to_start(const File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = 0;
			return SetFilePointerEx(handle.windows_handle, offset, &position, FILE_BEGIN);
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = 0;
			return lseek64(handle.linux_handle, offset, SEEK_SET) != -1;
		}
		#endif
	}

	bool
	OS::file_cursor_move_to_end(const File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = 0;
			return SetFilePointerEx(handle.windows_handle, offset, &position, FILE_END);
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = 0;
			return lseek64(handle.linux_handle, offset, SEEK_END) != -1;
		}
		#endif
	}

	fcontext_t
	OS::fcontext_make(const Slice<byte>& stack_memory, void(*proc)(intptr_t))
	{
		return make_fcontext(stack_memory.ptr + stack_memory.size, stack_memory.size, proc);
	}

	intptr_t
	OS::fcontext_jump(fcontext_t* out_fcontext,
					  fcontext_t in_fcontext,
					  intptr_t vp,
					  int preserve_fpu)
	{
		return jump_fcontext(out_fcontext, in_fcontext, vp, preserve_fpu);
	}


	//private functions

	//Memory Functions
	Owner<byte>
	_debug_memory_alloc(void* _self, usize size)
	{
		if(size == 0)
			return Owner<byte>();

		Memory_Block* ptr = (Memory_Block*)std::malloc(size + sizeof(Memory_Block));

		if(ptr == nullptr)
			return Owner<byte>();

		ptr->size = size;
		ptr->next = _leak_head;
		ptr->prev = nullptr;
		_leak_mtx.lock();
		{
			if(_leak_head) _leak_head->prev = ptr;
			_leak_head = ptr;
		}
		_leak_mtx.unlock();

		::new (&ptr->call_stack) Memory_Stream(&_mallocator);
		OS* os = (OS*)_self;
		os->dump_callstack(ptr->call_stack);

		return Owner<byte>((byte*)(ptr + 1), size);
	}

	void
	_debug_memory_free(void* _self, const Owner<byte>& value)
	{
		if(!value)
			return;

		Memory_Block* ptr = (Memory_Block*)(value.ptr - sizeof(Memory_Block));

		_leak_mtx.lock();
		{
			if(ptr == _leak_head)
				_leak_head = ptr->next;
			
			if(ptr->prev)
				ptr->prev->next = ptr->next;

			if(ptr->next)
				ptr->next->prev = ptr->prev;
		}
		_leak_mtx.unlock();

		ptr->call_stack.reset();

		std::free(ptr);
	}

	Owner<byte>
	_virtual_memory_alloc(void* _self, usize size)
	{
		if(size == 0)
			return Owner<byte>();

		OS* self = (OS*)_self;

		auto result = self->virtual_alloc(nullptr, size);

		if(result.empty())
			return Owner<byte>();

		return result;
	}

	void
	_virtual_memory_free(void* _self, const Owner<byte>& value)
	{
		if(!value)
			return;

		OS* self = (OS*)_self;

		self->virtual_free(value);
	}

	//IO Stuff
	usize
	_write_std_handle(void *self, const Slice<byte>& data)
	{
		File_Handle *handle = reinterpret_cast<File_Handle*>(self);

		#if defined(OS_WINDOWS)
		{
			constexpr usize BUFFER_SIZE = KILOBYTES(2);
			WCHAR buffer[BUFFER_SIZE];
			WCHAR* wide_ptr = nullptr;
			auto count_needed = MultiByteToWideChar(CP_UTF8, NULL, data.ptr, data.size, NULL, 0);

			Owner<WCHAR> dynamic_buffer;
			if(count_needed > BUFFER_SIZE)
			{
				dynamic_buffer = os->global_memory->template alloc<WCHAR>(count_needed);
				wide_ptr = dynamic_buffer.ptr;
			}
			else
			{
				wide_ptr = buffer;
			}

			MultiByteToWideChar(CP_UTF8, NULL, data.ptr, data.size, wide_ptr, count_needed);

			DWORD count_chars_written = 0;
			auto success = WriteConsoleW(handle->windows_handle, wide_ptr, count_needed, &count_chars_written, NULL);

			if(count_needed > BUFFER_SIZE)
				os->global_memory->free(dynamic_buffer);

			if(success)
				return data.size;
			else
				return 0;
		}
		#elif defined(OS_LINUX)
		{
			return os->file_write(*handle, data);
		}
		#endif
	}

	usize
	_read_std_handle(void* self, Slice<byte>& data)
	{
		File_Handle *handle = reinterpret_cast<File_Handle*>(self);

		#if defined(OS_WINDOWS)
		{
			constexpr usize BUFFER_SIZE = KILOBYTES(2);
			WCHAR buffer[BUFFER_SIZE];
			WCHAR* wide_ptr = buffer;
			usize wide_read = (data.size / sizeof(WCHAR)) < (BUFFER_SIZE / 2) ? (data.size / sizeof(WCHAR)) : (BUFFER_SIZE / 2);
			DWORD read_chars_count = 0;
			ReadConsoleW(handle->windows_handle, wide_ptr, wide_read, &read_chars_count, NULL);
			auto result = WideCharToMultiByte(CP_UTF8, NULL, wide_ptr, read_chars_count, data.ptr, data.size, NULL, NULL);
			return result;
		}
		#elif defined(OS_LINUX)
		{
			return os->file_read(*handle, data);
		}
		#endif
	}

	Dynamic_Array<Allocator_Trait*>*
	_allocators(Allocator_Trait* alloc = nullptr)
	{
		static thread_local Dynamic_Array<Allocator_Trait*> _allocs({ os->global_memory }, os->global_memory);
		return &_allocs;
	}

	//Init Stuff
	OS*
	_actual_init_os()
	{
		static Allocator_Trait _global_memory_trait, _virtual_memory_trait, _leak_detector_trait;
		static File_Handle _stdout_handle, _stderr_handle, _stdin_handle;
		static IO_Trait _stdout, _stderr, _stdin;
		static OS _os;
		static bool _is_initialized = false;

		if(_is_initialized)
			return &_os;

		//Get the standard handles
		#if defined(OS_WINDOWS)
		{

			constexpr const u32 MY_ENABLE_VIRTUAL_TERMINAL_PROCESSING = 4;

			_stdout_handle.windows_handle = GetStdHandle(STD_OUTPUT_HANDLE);
			_stderr_handle.windows_handle = GetStdHandle(STD_ERROR_HANDLE);
			_stdin_handle.windows_handle = GetStdHandle(STD_INPUT_HANDLE);

			DWORD mode;
			GetConsoleMode(_stdout_handle.windows_handle, &mode);
			mode |= MY_ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(_stdout_handle.windows_handle, mode);

			#if defined(DEBUG)
			{
				if(SymInitialize(GetCurrentProcess(), NULL, true))
					_os.debug_configured = true;
			}
			#endif
		}
		#elif defined(OS_LINUX)
		{
			_stdout_handle.linux_handle = STDOUT_FILENO;
			_stderr_handle.linux_handle = STDERR_FILENO;
			_stdin_handle.linux_handle = STDIN_FILENO;
		}
		#endif

		//setup the OS hooks
		_os.global_memory = &_global_memory_trait;
		_os.virtual_memory = &_virtual_memory_trait;
		_os.leak_detector = &_leak_detector_trait;
		_os.unbuf_stdout = &_stdout;
		_os.unbuf_stderr = &_stderr;
		_os.unbuf_stdin = &_stdin;

		//setup the global memory allocator
		_global_memory_trait._self  = &_os;
		_global_memory_trait._alloc = _malloc;
		_global_memory_trait._free 	= _free;

		//setup the leak detector allocator
		_leak_detector_trait._self = &_os;
		_leak_detector_trait._alloc = _debug_memory_alloc;
		_leak_detector_trait._free = _debug_memory_free;

		//setup the virtual memory allocator
		_virtual_memory_trait._self  = &_os;
		_virtual_memory_trait._alloc = _virtual_memory_alloc;
		_virtual_memory_trait._free  = _virtual_memory_free;

		//setup the stdout
		_stdout._self = &_stdout_handle;
		_stdout._write =  _write_std_handle;
		_stdout._read = nullptr;

		//setup the stderr
		_stderr._self = &_stderr_handle;
		_stderr._write =  _write_std_handle;
		_stderr._read = nullptr;

		//setup the stdin
		_stdin._self = &_stdin_handle;
		_stdin._write =  nullptr;
		_stdin._read = _read_std_handle;

		//setup the buffered stdin
		static Buf_Reader _buf_stdin(_os.unbuf_stdin, _os.global_memory);
		_os.buf_stdin = _buf_stdin;

		_is_initialized = true;

		return &_os;
	}

	OS *os = _actual_init_os();

	Allocator_Trait*
	allocator()
	{
		return _allocators()->back();
	}

	void
	allocator_push(Allocator_Trait* alloc)
	{
		_allocators()->insert_back(alloc);
	}

	Allocator_Trait*
	allocator_pop()
	{
		auto stack = _allocators();
		if(stack->count() == 1)
			return nullptr;
		auto res = stack->back();
		stack->remove_back();
		return res;
	}
}
