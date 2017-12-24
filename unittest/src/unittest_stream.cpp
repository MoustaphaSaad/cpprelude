#include "catch.hpp"
#include <cpprelude/stream.h>
#include <cpprelude/file.h>
#include <cpprelude/io.h>
#include <cpprelude/string.h>
#include <stdio.h>

using namespace cpprelude;

TEST_CASE("memory_stream test", "[memory_stream]")
{
	SECTION("Case 01")
	{
		memory_stream stream;

		CHECK(stream.size() == 0);
		CHECK(stream.cursor() == 0);
		CHECK(stream.capacity() == 0);
		CHECK(stream.empty() == true);
	}

	SECTION("Case 02")
	{
		memory_stream stream;

		int x = 7;
		CHECK(vprintb(stream, x) == sizeof(int));
		CHECK(vprints(stream, x) == 1);

		CHECK(stream.size() >= 5);
		CHECK(stream.capacity() >= 5);
		CHECK(stream.empty() == false);

		CHECK(stream.cursor() == 5);
		CHECK(stream.move(-1));
		CHECK(stream.cursor() == 4);

		char c;
		CHECK(vreadb(stream, c) == 1);
		CHECK(c == '7');

		CHECK(stream.cursor() == 5);
		CHECK(stream.move_to_start());
		CHECK(stream.cursor() == 0);

		int y = 1;
		CHECK(vreadb(stream, y) == sizeof(int));
		CHECK(y == x);

		CHECK(stream.cursor() == 4);
		CHECK(stream.move_to_end());
		CHECK(stream.cursor() == 5);
	}

	SECTION("Case 03")
	{
		memory_stream stream;

		CHECK(vprints(stream, -213) == 4);

		memory_stream other_stream = std::move(stream);
		CHECK(other_stream.size() >= 4);
		CHECK(other_stream.capacity() >= 4);
		CHECK(other_stream.cursor() == 4);
	}
}

TEST_CASE("file_stream test", "[file_stream]")
{
	SECTION("Case 01")
	{
		{
			file stream = file::open("file_stream"_cs, IO_MODE::WRITE);
			REQUIRE(stream.valid() == true);

			CHECK(stream.size() == 0);
			CHECK(stream.cursor() == 0);
		}
		remove("file_stream");
	}

	SECTION("Case 02")
	{
		{
			file stream = file::open("file_stream"_cs, IO_MODE::READ_WRITE);
			REQUIRE(stream.valid() == true);

			int x = 6;
			CHECK(vprintb(stream, x) == sizeof(int));
			CHECK(vprints(stream, x) == 1);

			CHECK(stream.size() >= 5);
			CHECK(stream.cursor() == 5);

			char c;
			CHECK(stream.move(-1));
			CHECK(stream.cursor() == 4);

			CHECK(vreadb(stream, c) == 1);
			CHECK(c == '6');

			CHECK(stream.cursor() == 5);

			CHECK(stream.move_to_start());
			CHECK(stream.cursor() == 0);

			int y = 1;
			CHECK(vreadb(stream, y) == sizeof(int));

			CHECK(y == x);

			CHECK(stream.cursor() == 4);
			CHECK(stream.move_to_end());
			CHECK(stream.cursor() == 5);
		}

		remove("file_stream");
	}

	SECTION("Case 03")
	{
		{
			file stream = file::open("file_stream"_cs, IO_MODE::WRITE);

			CHECK(vprints(stream, -213) == 4);

			file other_stream = std::move(stream);
			CHECK(other_stream.size() >= 4);
			CHECK(other_stream.cursor() == 4);
		}
		remove("file_stream");
	}
}