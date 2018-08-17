#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/Allocator_Trait.h"
#include "cpprelude/Result.h"
#include "cpprelude/Ranges.h"
#include "cpprelude/File_Handle.h"
#include "cpprelude/IO_Trait.h"
#include "sewing-fcontext/fcontext.h"
#include <utility>

namespace cppr
{
	/**
	 * [[markdown]]
	 * #OS
	 * Operation System layer is a thin shim between cpprelude and OS-specific functionality
	 */

	/**
	 * @brief      Enumeration for all OS error flags
	 * 
	 * - **OK**: No error flag
	 * - **GENERIC_ERROR**: Generic unspecfied error
	 * - **FILE_ALREADY_EXISTS**: File already exists error
	 * - **FILE_DOESNOT_EXIST**: File does not exist error
	 */
	enum class OS_ERROR
	{
		OK,
		GENERIC_ERROR,
		FILE_ALREADY_EXISTS,
		FILE_DOESNOT_EXIST
	};

	/**
	 * @brief      Represents the underlying operation system
	 */
	struct OS
	{
		/**
		 * The global memory context which uses the underlying malloc/free
		 */
		Allocator_Trait* global_memory;

		//used in case you want to check for leaks [it's slow]
		Allocator_Trait* leak_detector;

		/**
		 * The virtual memory context which uses the underlying OS specfic virtual alloc and free
		 * It's recommended to use this with Big allocations only i.e. 1GB of memory
		 */
		Allocator_Trait* virtual_memory;

		/**
		 * The unbuffered standard output IO Trait
		 */
		IO_Trait* unbuf_stdout;

		/**
		 * The unbuffered standard error IO Trait
		 */
		IO_Trait* unbuf_stderr;

		/**
		 * The unbuffered standard input IO Trait
		 */
		IO_Trait* unbuf_stdin;

		/**
		 * The buffered standard input Bufio_Trait
		 */
		Bufio_Trait* buf_stdin;
		bool debug_configured = false;

		API_CPPR ~OS();

		API_CPPR void
		print_leak_report(IO_Trait* io = nullptr) const;

		/**
		 * @brief      Dumps a callstack in debug mode, does nothing in release mode.
		 */
		API_CPPR void
		dump_callstack(IO_Trait* io = nullptr) const;

		/**
		 * @brief      Allocates memory from OS virtual memory
		 *
		 * @param      address_hint  The address hint
		 * @param[in]  size          The size of the memory in bytes
		 *
		 * @return     An Owner pointer to the underlying memory block
		 */
		API_CPPR Owner<byte>
		virtual_alloc(void* address_hint, usize size);

		/**
		 * @brief      Frees the underlying virtual memory of the given owner pointer
		 *
		 * @param      value  The owner pointer to free
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		virtual_free(const Owner<byte>& data);

		/**
		 * @brief      Opens a file
		 *
		 * @param[in]  name       The file name on disk
		 * @param[in]  io_mode    The i/o mode
		 * @param[in]  open_mode  The open mode
		 *
		 * @return     The result of file handle open which contains a file and an OS error
		 */
		API_CPPR Result<File_Handle, OS_ERROR>
		file_open(const String_Range& filename,
				  IO_MODE io_mode = IO_MODE::READ_WRITE,
				  OPEN_MODE open_mode = OPEN_MODE::CREATE_OVERWRITE);

		/**
		 * @brief      Closes a file handle
		 *
		 * @param      handle  The file handle
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		file_close(File_Handle& handle);

		/**
		 * @brief      Closes a file handle
		 *
		 * @param      handle  The file handle
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		file_close(File_Handle&& handle);

		/**
		 * @brief      Checks whether the file handle is valid or not
		 *
		 * @param[in]  handle  The file handle
		 *
		 * @return     Whether the file handle is valid or not
		 */
		API_CPPR bool
		file_valid(const File_Handle& handle);

		/**
		 * @brief      Writes data to a file handle
		 *
		 * @param[in]  handle  The file handle
		 * @param[in]  data    The data to be written
		 *
		 * @return     The size of the written data in bytes
		 */
		API_CPPR usize
		file_write(const File_Handle& handle, const Slice<byte>& data);

		/**
		 * @brief      Reads data from a file handle
		 *
		 * @param[in]  handle  The file handle
		 * @param      data    The data to be read
		 *
		 * @return     The size of the read data in bytes
		 */
		API_CPPR usize
		file_read(const File_Handle& handle, Slice<byte>& data);

		/**
		 * @brief      Reads data from a file handle
		 *
		 * @param[in]  handle  The file handle
		 * @param      data    The data to be read
		 *
		 * @return     The size of the read data in bytes
		 */
		API_CPPR usize
		file_read(const File_Handle& handle, Slice<byte>&& data);

		/**
		 * @brief      Returns the file size in bytes
		 *
		 * @param[in]  handle  The file handle
		 *
		 * @return     The size of the file in bytes
		 */
		API_CPPR i64
		file_size(const File_Handle& handle);

		/**
		 * @brief      Returns the file cursor position in bytes
		 *
		 * @param[in]  handle  The file handle
		 *
		 * @return     The file cursor position in bytes
		 */
		API_CPPR i64
		file_cursor_position(const File_Handle& handle);

		/**
		 * @brief      Moves the file cursor
		 *
		 * @param[in]  handle       The file handle
		 * @param[in]  move_offset  The move offset in bytes
		 * 
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		file_cursor_move(const File_Handle& handle, i64 move_offset);

		/**
		 * @brief      Moves the file cursor to the start of the file
		 *
		 * @param[in]  handle  The file handle
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		file_cursor_move_to_start(const File_Handle& handle);

		/**
		 * @brief      Moves the file cursor to the end of the file
		 *
		 * @param[in]  handle  The file handle
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		file_cursor_move_to_end(const File_Handle& handle);

		API_CPPR fcontext_t
		fcontext_make(const Slice<byte>& stack_memory, void(*proc)(intptr_t));

		API_CPPR intptr_t
		fcontext_jump(fcontext_t* out_fcontext, fcontext_t in_fcontext, intptr_t vp, int preserve_fpu = 1);
	};

	API_CPPR Allocator_Trait*
	allocator();

	API_CPPR void
	allocator_push(Allocator_Trait* alloc);

	API_CPPR Allocator_Trait*
	allocator_pop();

	template<typename T>
	Owner<T>
	alloc(usize count = 1)
	{
		return allocator()->template alloc<T>(count);
	}

	template<typename T>
	void
	free(Owner<T>& value)
	{
		return allocator()->template free<T>(value);
	}

	template<typename T>
	void
	free(Owner<T>&& value)
	{
		return allocator()->template free<T>(value);
	}

	template<typename T, typename ... TArgs>
	Owner<T>
	construct(usize count, TArgs&& ... args)
	{
		auto result = alloc<T>(count);
		for(usize i = 0; i < count; ++i)
			::new (result.ptr + i) T(std::forward<TArgs>(args)...);
		return result;
	}

	template<typename T>
	void
	destruct(Owner<T>& value)
	{
		for(usize i = 0; i < value.count(); ++i)
			value[i].~T();
		free(value);
	}

	template<typename T>
	void
	destruct(Owner<T>&& value)
	{
		for(usize i = 0; i < value.count(); ++i)
			value[i].~T();
		free(value);
	}

	/**
	 * A pointer to the singleton OS
	 */
	API_CPPR extern OS *os;
}
