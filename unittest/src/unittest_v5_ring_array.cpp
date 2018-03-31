#include "catch.hpp"
#include <cpprelude/v5/Ring_Array.h>

using namespace cpprelude;

TEST_CASE("ring_array v5", "[ring_array]")
{
	SECTION("Case 01")
	{
		Ring_Array<usize> array;
		CHECK(array.empty() == true);
		CHECK(array.count() == 0);
	}

	SECTION("Case 02")
	{
		Ring_Array<usize> array({1, 2, 3, 4});
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);
	}

	SECTION("Case 03")
	{
		Ring_Array<usize> array(5, 7);
		CHECK(array.empty() == false);
		CHECK(array.count() == 5);
	}

	SECTION("Case 04")
	{
		Ring_Array<usize> array({1, 2, 3, 4});
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		Ring_Array<usize> other(array);
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);
		CHECK(other.empty() == false);
		CHECK(other.count() == 4);
	}

	SECTION("Case 05")
	{
		Ring_Array<usize> array({1, 2, 3, 4});
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		Ring_Array<usize> other(std::move(array));
		CHECK(other.empty() == false);
		CHECK(other.count() == 4);
	}

	SECTION("Case 06")
	{
		Ring_Array<usize> array({1, 2, 3, 4});
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		Ring_Array<usize> other = array;
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);
		CHECK(other.empty() == false);
		CHECK(other.count() == 4);
	}

	SECTION("Case 07")
	{
		Ring_Array<usize> array({1, 2, 3, 4});
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		Ring_Array<usize> other = std::move(array);
		CHECK(other.empty() == false);
		CHECK(other.count() == 4);
	}

	SECTION("Case 08")
	{
		Ring_Array<usize> array;

		for(isize i = 9; i >= 0; --i)
			array.emplace_front(i);

		CHECK(array.count() == 10);
		CHECK(array.empty() == false);

		usize i = 0;
		while(!array.empty())
		{
			CHECK(i == array.front());
			array.remove_front();
			++i;
		}

		CHECK(array.count() == 0);
		CHECK(array.empty() == true);
	}

	SECTION("Case 09")
	{
		Ring_Array<usize> array;

		for(isize i = 9; i >= 0; --i)
			array.insert_front(i);

		CHECK(array.count() == 10);
		CHECK(array.empty() == false);

		usize i = 0;
		while(!array.empty())
		{
			CHECK(i == array.front());
			array.remove_front();
			++i;
		}

		CHECK(array.count() == 0);
		CHECK(array.empty() == true);
	}

	SECTION("Case 10")
	{
		Ring_Array<usize> array;

		for(isize i = 2; i >= 0; --i)
			array.insert_front(i);

		array.clear();

		CHECK(array.empty() == true);
		CHECK(array.count() == 0);
	}

	SECTION("Case 11")
	{
		Ring_Array<usize> array;

		for(isize i = 2; i >= 0; --i)
			array.insert_front(i);

		array.reset();

		CHECK(array.empty() == true);
		CHECK(array.count() == 0);
	}

	SECTION("Case 12")
	{
		Ring_Array<usize> array;

		for(usize i = 0; i < 10; ++i)
			array.emplace_back(i);

		usize i = 0;
		while(!array.empty())
		{
			CHECK(i == array.front());
			array.remove_front();
			++i;
		}

		CHECK(array.count() == 0);
		CHECK(array.empty() == true);
	}

	SECTION("Case 13")
	{
		Ring_Array<usize> array;

		for(usize i = 0; i < 10; ++i)
			array.insert_back(i);

		CHECK(array.count() == 10);
		CHECK(array.empty() == false);

		usize i = 0;
		while(!array.empty())
		{
			CHECK(i == array.front());
			array.remove_front();
			++i;
		}

		CHECK(array.count() == 0);
		CHECK(array.empty() == true);
	}
}