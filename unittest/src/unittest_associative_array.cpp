#include "catch.hpp"
#include <cpprelude/associative_array.h>

#include <iostream>
using namespace std;
using namespace cpprelude;


TEST_CASE("associative_array test", "[associative_array]")
{
	SECTION("Case 01")
	{
		using pair_node = details::pair_node<int, string>;

		associative_array <int, string> array;
		auto x = array.insert(5);
		CHECK(x->data.key == 5);
		CHECK(x->data.value == "");

		x = array.insert(10, "10");
		CHECK(x->data.key == 10);
		CHECK(x->data.value == "10");

		array.insert(1, "1");
		x = array.find(1);
		CHECK(x->data.key == 1);
		CHECK(x->data.value == "1");

		pair_node p(20, "20");
		array.insert(p);
		x = array.find(20);
		CHECK(x->data == p);

		CHECK(array.at(1) == "1");

		array[10] = "11";
		CHECK(array.at(10) == "11");

		CHECK(array.find(0) == nullptr);

		CHECK(array.get_min()->data.key == 1);
		CHECK(array.get_max()->data.key == 20);

		array.delete_min();
		CHECK(array.get_min()->data.key == 5);
		array.delete_max();
		CHECK(array.get_max()->data.key == 10);

	}
}