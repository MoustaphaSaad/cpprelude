#include "catch.hpp"
#include <cpprelude/stream.h>
#include <cpprelude/file.h>
#include <cpprelude/bufio.h>
#include <cpprelude/fmt.h>
#include <cpprelude/string.h>
#include <stdio.h>

using namespace cpprelude;

TEST_CASE("buf_reader test", "[buf_reader]")
{
	SECTION("Case 01")
	{
		memory_stream mem_stream;

		u64 x = 0;
		CHECK(vprintb(mem_stream, x) == sizeof(u64));

		x = 123;
		CHECK(vprintb(mem_stream, x) == sizeof(u64));

		CHECK(mem_stream.move_to_start() == true);

		buf_reader reader(mem_stream);
		CHECK(vscanb(reader, x) == sizeof(u64));
		CHECK(x == 0);
		CHECK(vscanb(reader, x) == sizeof(u64));
		CHECK(x == 123);
	}

	SECTION("Case 02")
	{
		memory_stream mem_stream;

		CHECK(vprints(mem_stream, "my name is mostafa\n") != 0);
		CHECK(vprints(mem_stream, "my age is 24\n") != 0);

		CHECK(mem_stream.move_to_start() == true);

		buf_reader reader(mem_stream);
		string str;

		CHECK(vscanln(reader, str) != 0);
		CHECK(str == "my name is mostafa"_cs);

		CHECK(vscanln(reader, str) != 0);
		CHECK(str == "my age is 24"_cs);

		CHECK(vscanln(reader, str) == 0);
	}
}

TEST_CASE("buf_writer test", "[buf_writer]")
{
	SECTION("Case 01")
	{
		memory_stream mem_stream;

		buf_writer writer(mem_stream);

		u64 x = 0;
		CHECK(vprintb(writer, x) == sizeof(u64));

		x = 1234;
		CHECK(vprintb(writer, x) == sizeof(u64));

		CHECK(vprints(writer, x) == 4);

		CHECK(writer.flush() <= (2 * sizeof(u64)) + 4);

		CHECK(writer.available() == 0);
	}
}
