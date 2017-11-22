#include "catch.hpp"
#include <cpprelude/slinked_list.h>

using namespace cpprelude;

TEST_CASE("slinked_list test", "[slinked_list]")
{
	slinked_list<i32> array;

	SECTION("Case 01")
	{
		CHECK(array.count() == 0);

		for(usize i = 0; i < 128; ++i)
			array.insert_front(i);

		usize i_ = 0;
		for(isize i = 128 - 1; i >= 0; --i)
			CHECK(array[i] == i_++);
	}

	SECTION("Case 02")
	{
		CHECK(array.count() == 0);

		array.expand_front(100, -5);

		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == -5);
	}

	SECTION("Case 03")
	{
		CHECK(array.count() == 0);

		for(usize i = 0; i < 128; ++i)
			array.insert_front(i);

		usize i_ = 0;
		for(isize i = 128-1; i >= 0; --i)
			CHECK(array[i] == i_++);

		CHECK(array.count() == 128);
	}

	SECTION("Case 04")
	{
		slinked_list<i32> array2;

		CHECK(array.count() == 0);
		CHECK(array2.count() == 0);

		array2 = array;

		CHECK(array.count() == 0);
		CHECK(array2.count() == 0);

		for(i32 i = 0; i < 64; ++i)
			array.insert_front(i);

		CHECK(array.count() == 64);

		array2 = array;

		usize i_ = 0;
		for(i32 i = 64-1; i >= 0; --i)
			CHECK(array2[i] == i_++);

		CHECK(array2.count() == array.count());
	}

	SECTION("Case 05")
	{

		CHECK(array.count() == 0);

		for(i32 i = 0; i < 64; ++i)
			array.insert_front(i);

		CHECK(array.count() == 64);

		slinked_list<i32> array2(array);

		usize i_ = 0;
		for(i32 i = 64-1; i >= 0; --i)
			CHECK(array2[i] == i_++);

		i_ = 64-1;
		for(i32 i = 0; i < 64; ++i)
			CHECK(array[i] == i_--);

		CHECK(array2.count() == array.count());
	}

	SECTION("Case 06")
	{

		CHECK(array.count() == 0);

		for(usize i = 0; i < 64; ++i)
			array.insert_front(i);

		CHECK(array.count() == 64);

		slinked_list<i32> array2(std::move(array));

		usize i_ = 64 - 1;
		for(usize i = 0; i < 64; ++i)
			CHECK(array2[i] == i_--);

		i_ = array.count() -1;
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i_--);

		CHECK(array2.count() != array.count());
		CHECK(array2.count() == 64);
		CHECK(array.count() == 0);
	}

	SECTION("Case 07")
	{
		slinked_list<i32> array2;

		CHECK(array.count() == 0);
		CHECK(array2.count() == 0);

		array2 = array;

		CHECK(array.count() == 0);
		CHECK(array2.count() == 0);

		for(i32 i = 0; i < 64; ++i)
			array.insert_front(i);

		CHECK(array.count() == 64);

		array2 = std::move(array);

		usize i_ = 64 - 1;
		for(i32 i = 0; i < 64; ++i)
			CHECK(array2[i] == i_--);

		CHECK(array2.count() != array.count());
		CHECK(array2.count() == 64);
		CHECK(array.count() == 0);
	}

	SECTION("Case 08")
	{
		slinked_list<i32> array2 = {1, 2, 3, 4, 5};
		for(usize i = 0; i < array2.count(); ++i)
			CHECK(array2[i] == i+1);
	}

	SECTION("Case 09")
	{
		array.insert_front({1, 2, 3, 4, 5});

		CHECK(array.count() == 5);
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i+1);
	}

	SECTION("Case 10")
	{
		array.insert_front({1, 2, 3, 4, 5});

		CHECK(array.count() == 5);
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i+1);

		array.insert_front({6, 7, 8, 9});
		array.insert_front(10);

		CHECK(array[0] == 10);
		CHECK(array[1] == 6);
		CHECK(array[2] == 7);
		CHECK(array[3] == 8);
		CHECK(array[4] == 9);
		CHECK(array[5] == 1);
		CHECK(array[6] == 2);
		CHECK(array[7] == 3);
		CHECK(array[8] == 4);
		CHECK(array[9] == 5);

		array.reset();
		CHECK(array.count() == 0);

	}

	SECTION("Case 11")
	{
		array.insert_front({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

		usize i = 1;
		for(auto number: array)
			CHECK(number == i++);

		i=1;
		for(auto& number: array)
			CHECK(number == i++);

		i=1;
		for(const auto& number: array)
			CHECK(number == i++);
	}

	SECTION("Case 12")
	{
		CHECK(array.begin() == array.end());
		CHECK(array.empty() == true);

		array.insert_front({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

		CHECK(array.empty() == false);

		auto it = array.begin();
		auto count = array.count();
		usize i = 1;
		while(count--)
		{
			CHECK(*it == i++);
			it = cpprelude::next(it);
		}

		CHECK(it == array.end());
		it = cpprelude::next(it);
		it = cpprelude::next(it);
		CHECK(it == array.end());
		CHECK(array.begin() != array.end());
	}

	SECTION("Case 13")
	{
		array.insert_front({1, 2, 3, 4, 5});

		CHECK(array.count() == 5);

		array.remove_front(2);
		
		CHECK(array.count() == 3);

		usize i = 3;
		for(auto number: array)
			CHECK(number == i++);
	}

	SECTION("Case 14")
	{
		array.insert_front({1, 2, 3, 4, 5, 6});

		for(auto it = array.begin(); it != array.end(); ++it)
		{
			array.insert_after(it, 0);
			++it;
		}

		for(auto it = array.begin(); it != array.end(); ++it)
		{
			CHECK(*it != 0);
			++it;
			CHECK(*it == 0);
		}
	}
}