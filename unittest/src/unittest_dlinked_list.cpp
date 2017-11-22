#include "catch.hpp"
#include <cpprelude/dlinked_list.h>

using namespace cpprelude;

TEST_CASE("dlinked_list test", "[dlinked_list]")
{
	dlinked_list<i32> array;

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
		dlinked_list<i32> array2;

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

		dlinked_list<i32> array2(array);

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

		for(i32 i = 0; i < 64; ++i)
			array.insert_front(i);

		CHECK(array.count() == 64);

		dlinked_list<i32> array2(std::move(array));

		usize i_ = 64 - 1;
		for(i32 i = 0; i < 64; ++i)
			CHECK(array2[i] == i_--);

		CHECK(array2.count() != array.count());
		CHECK(array2.count() == 64);
		CHECK(array.count() == 0);
	}

	SECTION("Case 07")
	{
		dlinked_list<i32> array2;

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
		dlinked_list<i32> array2 = {1, 2, 3, 4, 5};
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
		CHECK(array.count() == 0);

		for(usize i = 0; i < 128; ++i)
			array.insert_back(i);

		for(usize i = 0; i < 128; ++i)
			CHECK(array[i] == i);
	}

	SECTION("Case 15")
	{
		CHECK(array.count() == 0);

		array.expand_back(100, -5);

		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == -5);
	}

	SECTION("Case 16")
	{
		CHECK(array.count() == 0);

		for(usize i = 0; i < 128; ++i)
			array.insert_back(i);

		for(usize i = 0; i < 128; ++i)
			CHECK(array[i] == i);

		CHECK(array.count() == 128);
		array.remove_back(array.count());
		CHECK(array.count() == 0);
		CHECK(array.begin() == array.end());
	}

	SECTION("Case 17")
	{
		CHECK(array.count() == 0);

		array.expand_back(64, -1);

		auto it = array.begin();
		for(usize i = 0; i < array.count(); ++i)
			CHECK(*(it++) == -1);

		CHECK(array.count() == 64);
		array.remove_front(array.count());
		CHECK(array.count() == 0);
		CHECK(array.begin() == array.end());	
	}

	SECTION("Case 18")
	{
		dlinked_list<i32> array2;

		CHECK(array.count() == 0);
		CHECK(array2.count() == 0);

		array2 = array;

		CHECK(array.count() == 0);
		CHECK(array2.count() == 0);

		for(i32 i = 0; i < 64; ++i)
			array.insert_back(i);

		CHECK(array.count() == 64);

		array2 = array;

		for(i32 i = 0; i < 64; ++i)
			CHECK(array2[i] == i);

		CHECK(array2.count() == array.count());
	}

	SECTION("Case 19")
	{

		CHECK(array.count() == 0);

		for(i32 i = 0; i < 64; ++i)
			array.insert_back(i);

		CHECK(array.count() == 64);

		dlinked_list<i32> array2(array);

		for(i32 i = 0; i < 64; ++i)
			CHECK(array2[i] == i);

		for(i32 i = 0; i < 64; ++i)
			CHECK(array[i] == i);

		CHECK(array2.count() == array.count());
	}

	SECTION("Case 20")
	{
		CHECK(array.count() == 0);

		for(usize i = 0; i < 64; ++i)
			array.insert_back(i);

		CHECK(array.count() == 64);

		dlinked_list<i32> array2(std::move(array));

		for(usize i = 0; i < 64; ++i)
			CHECK(array2[i] == i);

		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i);

		CHECK(array2.count() != array.count());
		CHECK(array2.count() == 64);
		CHECK(array.count() == 0);
	}

	SECTION("Case 21")
	{
		dlinked_list<i32> array2;

		CHECK(array.count() == 0);
		CHECK(array2.count() == 0);

		array2 = array;

		CHECK(array.count() == 0);
		CHECK(array2.count() == 0);

		for(i32 i = 0; i < 64; ++i)
			array.insert_back(i);

		CHECK(array.count() == 64);

		array2 = std::move(array);

		for(i32 i = 0; i < 64; ++i)
			CHECK(array2[i] == i);

		CHECK(array2.count() != array.count());
		CHECK(array2.count() == 64);
		CHECK(array.count() == 0);
	}

	SECTION("Case 22")
	{
		dlinked_list<i32> array2 = {1, 2, 3, 4, 5};
		for(usize i = 0; i < array2.count(); ++i)
			CHECK(array2[i] == i+1);
	}

	SECTION("Case 23")
	{
		array.insert_back({1, 2, 3, 4, 5});

		CHECK(array.count() == 5);
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i+1);

		array.insert_back({6, 7, 8, 9});
		array.insert_back(10);

		CHECK(array.count() == 10);
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i+1);
	}

	SECTION("Case 24")
	{
		array.insert_back({1, 2, 3, 4, 5});

		CHECK(array.count() == 5);
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i+1);

		array.insert_back({6, 7, 8, 9});
		array.insert_back(10);

		CHECK(array.count() == 10);
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i+1);

		array.reset();
		CHECK(array.count() == 0);

		array.insert_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
		CHECK(array.count() == 10);

		array.reset();
		CHECK(array.count() == 0);

	}

	

	SECTION("Case 25")
	{
		array.insert_back({1, 2, 3, 4, 5, 6, 7, 8, 9});

		usize i = 1;
		for(auto number: array)
			CHECK(number == i++);

		i = 1;
		for(auto& number: array)
			CHECK(number == i++);

		i = 1;
		for(const auto& number: array)
			CHECK(number == i++);

		for(auto& number: array)
			number++;

		i = 2;
		for(const auto& number: array)
			CHECK(number == i++);
	}

	SECTION("Case 26")
	{
		CHECK(array.begin() == array.end());
		CHECK(array.empty() == true);

		array.insert_back({1, 2, 3, 4});

		CHECK(array.empty() == false);
		CHECK(array.begin() != array.end());

		auto it = array.begin();
		CHECK(it == array.begin());
		it = cpprelude:: next(it);
		CHECK(it != array.begin());
		it = cpprelude::prev(it);
		CHECK(it == array.begin());

		it = cpprelude::next(it);
		it = cpprelude::next(it);
		it = cpprelude::next(it);
		CHECK(it != array.begin());
		it = cpprelude::next(it);
		it = cpprelude::next(it);
		it = cpprelude::next(it);
		it = cpprelude::next(it);
		CHECK(it == array.end());
	}

	SECTION("Case 27")
	{
		for(usize i = 0; i < 64; i++)
			array.insert_back(i);

		usize i = 0;
		for(auto number: array)
			CHECK(number == i++);

		array.shrink_back(32);
		CHECK(array.count() == 32);

		i = 0;
		for(auto number: array)
			CHECK(number == i++);
	}

	SECTION("Case 28")
	{
		array.insert_back({1, 2, 3, 4, 5});
		array.remove_back(2);
		CHECK(array.count() == 3);
		usize i = 1;
		for(auto number: array)
			CHECK(number == i++);
	}

	SECTION("Case 29")
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

	SECTION("Case 30")
	{
		array.insert_front({1, 2, 3, 4, 5, 6});

		for(auto it = array.begin(); it != array.end(); ++it)
		{
			array.insert_before(it, 0);
		}

		for(auto it = array.begin(); it != array.end(); ++it)
		{
			CHECK(*it == 0);
			++it;
			CHECK(*it != 0);
		}
	}

	SECTION("Case 31")
	{
		array.insert_front({ 1, 2, 3, 4, 5, 6 });

		auto it = array.begin();
		array.remove(it);
		
		usize i = 2;
		for (auto it = array.begin(); it != array.end(); ++it)
		{
			CHECK(*it == i);
			++i;
		}
	}

	SECTION("Case 32")
	{
		array.insert_front({ 1, 2, 3, 4, 5, 6 });

		auto it = array.begin();
		array.remove(it);

		it = --array.end();
		array.remove(it);

		usize i = 2;
		for (auto it = array.begin(); it != array.end(); ++it)
		{
			CHECK(*it == i);
			++i;
		}

		it = ++(++array.begin());
		array.remove(it);

		i = 2;
		for (auto it = array.begin(); it != array.end(); ++it)
		{
			if (i == 4)
				++i;
			CHECK(*it == i);
			++i;
		}

	}
}
