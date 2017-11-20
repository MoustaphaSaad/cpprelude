#include "catch.hpp"
#include <cpprelude/stream.h>
#include <cpprelude/string.h>
#include <stdio.h>

using namespace cpprelude;

TEST_CASE("memory_stream test", "[memory_stream]")
{
	SECTION("Case 01")
	{
		memory_stream stream;

		CHECK(stream.size() == 0);
		CHECK(stream.capacity() == 0);
		CHECK(stream.write_capacity() == 0);
		CHECK(stream.read_position() == 0);
		CHECK(stream.empty() == true);
	}

	SECTION("Case 02")
	{
		memory_stream stream;

		int x = 0;
		CHECK(write(stream, x) == sizeof(int));
		CHECK(write_str(stream, x) == 1);

		CHECK(stream.size() >= 5);
		CHECK(stream.capacity() >= 5);
		CHECK(stream.empty() == false);

		auto buffer = platform.alloc<byte>(4);
		CHECK(read(stream, buffer) == 4);
		platform.free(buffer);

		CHECK(stream.read_position() == 4);
	}

	SECTION("Case 03")
	{
		memory_stream stream;

		CHECK(write_str(stream, -123) > 0);
		memory_stream other_stream = std::move(stream);
		CHECK(other_stream._trait._self == &other_stream);
		CHECK(other_stream.size() >= 4);
		CHECK(other_stream.capacity() >= 4);
		CHECK(other_stream.read_capacity() == 4);
	}
}

TEST_CASE("file_stream test", "[file_stream]")
{
	SECTION("Case 01")
	{
		{
			file_stream stream = open_file("file_stream01"_cs, IO_MODE::WRITE);

			REQUIRE(stream.valid() == true);
			CHECK(stream.size() == 0);
			CHECK(stream.read_position() == 0);
		}
		remove("file_stream01");
	}

	SECTION("Case 02")
	{
		{
			file_stream stream = open_file("file_stream02"_cs, IO_MODE::WRITE_EXTENDED);
			REQUIRE(stream.valid() == true);

			int x = 0;
			CHECK(write(stream, x) == sizeof(int));
			CHECK(write_str(stream, x) == 1);

			CHECK(stream.size() >= 5);

			auto buffer = platform.alloc<byte>(4);
			CHECK(read(stream, buffer) == 4);
			platform.free(buffer);

			CHECK(stream.read_position() == 4);
		}

		remove("file_stream02");
	}

	SECTION("Case 03")
	{
		{
			file_stream stream = open_file("file_stream03"_cs, IO_MODE::WRITE_EXTENDED);
			REQUIRE(stream.valid() == true);

			CHECK(write_str(stream, -123) > 0);

			file_stream other_stream = std::move(stream);
			CHECK(other_stream.valid() == true);
			CHECK(stream.valid() == false);

			CHECK(other_stream._trait._self == &other_stream);

			CHECK(other_stream.size() >= 4);
			CHECK(other_stream.read_position() == 0);
		}
		remove("file_stream03");
	}
}