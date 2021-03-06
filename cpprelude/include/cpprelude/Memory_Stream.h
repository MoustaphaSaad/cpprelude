#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/Dynamic_Array.h"
#include "cpprelude/IO_Trait.h"
#include "cpprelude/OS.h"

namespace cppr
{
	/**
	 * @brief      A Memory IO stream implementation
	 */
	struct Memory_Stream
	{
		Dynamic_Array<byte> _buffer;
		i64 _cursor;
		IO_Trait _io_trait;

		/**
		 * @brief      Constructs a memory stream
		 *
		 * @param      context  The memory context to use for allocation and freeing
		 */
		API_CPPR Memory_Stream(Allocator_Trait* context = allocator());

		/**
		 * @brief      Constructs a memory stream
		 *
		 * @param[in]  data     An array of data to start with
		 * @param      context  The memory context to use for allocation and freeing
		 */
		API_CPPR Memory_Stream(const Dynamic_Array<byte>& data,
							   Allocator_Trait* context = allocator());

		/**
		 * @brief      Constructs a memory stream
		 *
		 * @param[in]  data     An array of data to start with
		 */
		API_CPPR Memory_Stream(Dynamic_Array<byte>&& data);

		/**
		 * Copy constructor is deleted
		 */
		Memory_Stream(const Memory_Stream&) = delete;

		/**
		 * Copy assignment operator is deleted
		 */
		Memory_Stream&
		operator=(const Memory_Stream&) = delete;

		/**
		 * @brief      Move constructor
		 */
		API_CPPR Memory_Stream(Memory_Stream&& other);

		/**
		 * @brief      Move assignment operator
		 */
		API_CPPR Memory_Stream&
		operator=(Memory_Stream&& other);

		/**
		 * @brief      Implicit cast operator to IO_Trait
		 */
		inline
		operator IO_Trait*()
		{
			return &_io_trait;
		}

		/**
		 * @return     Stream size in bytes
		 */
		API_CPPR usize
		size() const;

		/**
		 * @return     Stream cursor position in bytes
		 */
		API_CPPR i64
		cursor() const;

		/**
		 * @brief      Moves the file cursor with the given offset
		 *
		 * @param[in]  offset  The offset to move the cursor by
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		move(i64 offset);

		/**
		 * @brief      Moves the file cursor to the start of the file
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		move_to_start();

		/**
		 * @brief      Moves the file cursor to the end of the file
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		move_to_end();

		/**
		 * @return     True if empty, false otherwise
		 */
		API_CPPR bool
		empty() const;

		/**
		 * @brief      Clears the memory stream
		 */
		API_CPPR void
		clear();

		/**
		 * @brief      Clears the memory stream and frees the memory
		 */
		API_CPPR void
		reset();

		/**
		 * @return     The capacity in bytes of this stream
		 */
		API_CPPR usize
		capacity() const;

		/**
		 * @brief      Ensures that the stream has the capacity for the expected count of bytes
		 *
		 * @param[in]  expected_count  The expected count in bytes to reserve
		 */
		API_CPPR void
		reserve(usize expected_size);

		/**
		 * @brief      Writes the given slice of data into the file
		 *
		 * @param[in]  data  The data to write
		 *
		 * @return     The size of the written data in bytes
		 */
		API_CPPR usize
		write(const Slice<byte>& data);

		/**
		 * @brief      Reads from the file to the given slice
		 *
		 * @param[in]  data  The data slice to read into
		 *
		 * @return     The size of the written data in bytes
		 */
		API_CPPR usize
		read(Slice<byte>& data);

		/**
		 * @brief      Reads from the file to the given slice
		 *
		 * @param[in]  data  The data slice to read into
		 *
		 * @return     The size of the written data in bytes
		 */
		API_CPPR usize
		read(Slice<byte>&& data);

		/**
		 * @brief Returns the content (from the start to the cursor) of the stream as a binary slice
		 * 
		 * @return Slice<byte>
		 */
		API_CPPR Slice<byte>
		bin_content();

		/**
		 * @brief Returns the content (from the start to the cursor) of the stream as a string range
		 * 
		 * @return String_Range
		 */
		API_CPPR String_Range
		str_content() const;

		/**
		 * @brief Returns the content (from the cursor to the end) of the stream as a binary slice
		 * 
		 * @return Slice<byte>
		 */
		API_CPPR Slice<byte>
		bin_ahead_cursor();

		/**
		 * @brief Returns the content (from the cursor to the end) of the stream as a string range
		 * 
		 * @return String_Range
		 */
		API_CPPR String_Range
		str_ahead_cursor() const;

		/**
		 * @brief Expands back the stream and returns the slice of the expanded memory to write into it
		 * 
		 * @param size The size of the memory to expand
		 * @return usize The size of the data that's piped into the stream (in bytes)
		 */
		API_CPPR usize
		pipe_in(IO_Trait* io, usize size);

		/**
		 * @brief Pipes the content of the stream into the provided IO_Trait
		 * 
		 * @param io The IO_Trait to pipe(write) the content to
		 * @param size The size of the data to be piped(written)
		 * @return usize The size of the piped data (in bytes)
		 */
		API_CPPR usize
		pipe_out(IO_Trait* io, usize size);
	};
}