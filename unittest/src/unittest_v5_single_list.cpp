#include "catch.hpp"
#include <cpprelude/Single_List.h>

using namespace cppr;

TEST_CASE("single_list", "[single_list]")
{
	SECTION("Case 01")
	{
		Single_List<usize> list;
		CHECK(list.empty() == true);
		CHECK(list.count() == 0);
	}

	SECTION("Case 02")
	{
		Single_List<usize> list({1, 2, 3, 4});
		CHECK(list.empty() == false);
		CHECK(list.count() == 4);

		usize i = 1;
		for(auto range = list.all(); !range.empty(); range.pop_front())
		{
			CHECK(i == range.front());
			++i;
		}
	}

	SECTION("Case 03")
	{
		Single_List<usize> list(5, 7);
		CHECK(list.empty() == false);
		CHECK(list.count() == 5);

		for(auto range = list.all(); !range.empty(); range.pop_front())
			CHECK(range.front() == 7);
	}

	SECTION("Case 04")
	{
		Single_List<usize> list({1, 2, 3, 4});
		CHECK(list.empty() == false);
		CHECK(list.count() == 4);

		Single_List<usize> other(list);
		CHECK(list.empty() == false);
		CHECK(list.count() == 4);
		CHECK(other.empty() == false);
		CHECK(other.count() == 4);

		usize i = 1;
		for(auto range = other.all(); !range.empty(); range.pop_front())
		{
			CHECK(i == range.front());
			++i;
		}
	}

	SECTION("Case 05")
	{
		Single_List<usize> list({1, 2, 3, 4});
		CHECK(list.empty() == false);
		CHECK(list.count() == 4);

		Single_List<usize> other(std::move(list));
		CHECK(other.empty() == false);
		CHECK(other.count() == 4);

		usize i = 1;
		for(auto range = other.all(); !range.empty(); range.pop_front())
		{
			CHECK(i == range.front());
			++i;
		}
	}

	SECTION("Case 06")
	{
		Single_List<usize> list({1, 2, 3, 4});
		CHECK(list.empty() == false);
		CHECK(list.count() == 4);

		Single_List<usize> other({1, 2, 3});
		other = list;
		CHECK(list.empty() == false);
		CHECK(list.count() == 4);
		CHECK(other.empty() == false);
		CHECK(other.count() == 4);

		usize i = 1;
		for(auto range = other.all(); !range.empty(); range.pop_front())
		{
			CHECK(i == range.front());
			++i;
		}
	}

	SECTION("Case 07")
	{
		Single_List<usize> list({1, 2, 3, 4});
		CHECK(list.empty() == false);
		CHECK(list.count() == 4);

		Single_List<usize> other = std::move(list);
		CHECK(other.empty() == false);
		CHECK(other.count() == 4);

		usize i = 1;
		for(auto range = other.all(); !range.empty(); range.pop_front())
		{
			CHECK(i == range.front());
			++i;
		}

		i = 1;
		for(auto range = list.all(); !range.empty(); range.pop_front())
		{
			CHECK(i == range.front());
			++i;
		}
	}

	SECTION("Case 08")
	{
		Single_List<usize> list;
		CHECK(list.count() == 0);
		CHECK(list.empty() == true);

		list.expand_front(4, 7);
		CHECK(list.count() == 4);
		CHECK(list.empty() == false);

		for(auto range = list.all(); !range.empty(); range.pop_front())
			CHECK(range.front() == 7);
	}

	SECTION("Case 09")
	{
		Single_List<usize> list;
		CHECK(list.count() == 0);
		CHECK(list.empty() == true);

		list.expand_front(4, 7);
		CHECK(list.count() == 4);
		CHECK(list.empty() == false);

		list.shrink_front(100);
		CHECK(list.count() == 0);
		CHECK(list.empty() == true);

		for(auto range = list.all(); !range.empty(); range.pop_front())
			CHECK(false);
	}

	SECTION("Case 10")
	{
		Single_List<usize> list;

		for(isize i = 9; i >= 0; --i)
			list.emplace_front(i);

		usize i = 0;
		for(auto range = list.all(); !range.empty(); range.pop_front())
		{
			CHECK(i == range.front());
			++i;
		}

		CHECK(list.count() == 10);
		CHECK(list.empty() == false);
	}

	SECTION("Case 11")
	{
		Single_List<usize> list;

		for(isize i = 9; i >= 0; --i)
			list.insert_front(i);

		usize i = 0;
		for(auto range = list.all(); !range.empty(); range.pop_front())
		{
			CHECK(i == range.front());
			++i;
		}
		CHECK(list.count() == 10);
		CHECK(list.empty() == false);
	}

	SECTION("Case 12")
	{
		Single_List<usize> list;

		for(isize i = 2; i >= 0; --i)
			list.insert_front(i);

		auto range = list.all();
		range.pop_front();
		for(isize i = 2; i >= 0; --i)
			list.emplace_after(range.begin(), i);

		auto it = list.all().begin();
		CHECK(*it == 0);
		++it;
		CHECK(*it == 1);
		++it;
		CHECK(*it == 0);
		++it;
		CHECK(*it == 1);
		++it;
		CHECK(*it == 2);
		++it;
		CHECK(*it == 2);
		++it;

		CHECK(list.count() == 6);
		CHECK(list.empty() == false);
	}

	SECTION("Case 13")
	{
		Single_List<usize> list;

		for(isize i = 2; i >= 0; --i)
			list.insert_front(i);

		auto range = list.all();
		range.pop_front();
		for(isize i = 2; i >= 0; --i)
			list.insert_after(range.begin(), i);

		auto it = list.all().begin();
		CHECK(*it == 0);
		++it;
		CHECK(*it == 1);
		++it;
		CHECK(*it == 0);
		++it;
		CHECK(*it == 1);
		++it;
		CHECK(*it == 2);
		++it;
		CHECK(*it == 2);
		++it;

		CHECK(list.count() == 6);
		CHECK(list.empty() == false);
	}

	SECTION("Case 14")
	{
		Single_List<usize> list;

		for(isize i = 2; i >= 0; --i)
			list.insert_front(i);

		list.remove_front();
		usize i = 1;
		for(auto range = list.all(); !range.empty(); range.pop_front())
		{
			CHECK(range.front() == i);
			++i;
		}

		CHECK(list.empty() == false);
		CHECK(list.count() == 2);
	}

	SECTION("Case 15")
	{
		Single_List<usize> list;

		for(isize i = 2; i >= 0; --i)
			list.insert_front(i);

		list.clear();

		CHECK(list.empty() == true);
		CHECK(list.count() == 0);
	}

	SECTION("Case 16")
	{
		Single_List<usize> list;

		for(isize i = 2; i >= 0; --i)
			list.insert_front(i);

		list.reset();

		CHECK(list.empty() == true);
		CHECK(list.count() == 0);
	}
}