#include <catch.hpp>
#include <cpprelude/String.h>
using namespace cppr;

TEST_CASE("string", "[string]")
{
	SECTION("Case 01")
	{
		String str;
		CHECK(str.size() == 0);
		CHECK(str.count() == 0);
		CHECK(str.empty() == true);
		CHECK(str.capacity() == 0);
	}

	SECTION("Case 02")
	{
		String str;
		str.reserve(7);
		CHECK(str.size() == 0);
		CHECK(str.count() == 0);
		CHECK(str.empty() == true);
		CHECK(str.capacity() >= 7);
	}

	SECTION("Case 03")
	{
		const char* name = "Mostafa";
		String str(name);
		CHECK(str.size() == 8);
		CHECK(str.count() == 7);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 8);
	}

	SECTION("Case 04")
	{
		const char* name = "Mostafa";
		String str(make_strrng(name, 3));
		CHECK(str.size() == 4);
		CHECK(str.count() == 3);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 4);
	}

	SECTION("Case 05")
	{
		auto data = alloc<byte>(3);
		data[0] = 'm';
		data[1] = 'o';
		data[2] = 's';
		String str(data);
		CHECK(str.size() == 3);
		CHECK(str.count() == 3);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 3);
		free(data);
	}

	SECTION("Case 06")
	{
		auto data = alloc<byte>(3);
		data[0] = 'm';
		data[1] = 'o';
		data[2] = 's';
		String str(std::move(data));
		CHECK(str.size() == 3);
		CHECK(str.count() == 3);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 3);
	}

	SECTION("Case 07")
	{
		String str("Mostafa");
		CHECK(str.size() == 8);
		CHECK(str.count() == 7);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 8);

		String other(str);
		CHECK(other.size() == 8);
		CHECK(other.count() == 7);
		CHECK(other.empty() == false);
		CHECK(other.capacity() >= 8);

		CHECK(str == other);
	}

	SECTION("Case 08")
	{
		String str("Mostafa");
		CHECK(str.size() == 8);
		CHECK(str.count() == 7);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 8);

		String other(std::move(str));
		CHECK(other.size() == 8);
		CHECK(other.count() == 7);
		CHECK(other.empty() == false);
		CHECK(other.capacity() >= 8);

		CHECK(other != "Mstafa");
		CHECK(other == "Mostafa");
	}

	SECTION("Case 09")
	{
		String str("Mostafa");
		CHECK(str.size() == 8);
		CHECK(str.count() == 7);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 8);

		String other("saad");
		CHECK(other.size() == 5);
		CHECK(other.count() == 4);
		CHECK(other.empty() == false);
		CHECK(other.capacity() >= 5);

		CHECK(other != str);

		other = str;
		CHECK(other.size() == 8);
		CHECK(other.count() == 7);
		CHECK(other.empty() == false);
		CHECK(other.capacity() >= 8);

		CHECK(str == other);
	}

	SECTION("Case 10")
	{
		String str("Mostafa");
		CHECK(str.size() == 8);
		CHECK(str.count() == 7);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 8);

		String other("saad");
		CHECK(other.size() == 5);
		CHECK(other.count() == 4);
		CHECK(other.empty() == false);
		CHECK(other.capacity() >= 5);

		CHECK(other != str);

		other = "Mostafa";
		CHECK(other.size() == 8);
		CHECK(other.count() == 7);
		CHECK(other.empty() == false);
		CHECK(other.capacity() >= 8);

		CHECK(str == other);
	}

	SECTION("Case 11")
	{
		String cpp_str[6] = {"abcd", "ab", "ba", "dcba", "fegh", "cdab"};
		std::string std_str[6] = {"abcd", "ab", "ba", "dcba", "fegh", "cdab"};

		for(usize i = 0; i < 6; ++i)
		{
			for(usize j = 0; j < 6; ++j)
			{
				auto a 	= cpp_str[i] == cpp_str[j];
				auto as = std_str[i] == std_str[j];
				CHECK(a == as);

				a  = cpp_str[i] != cpp_str[j];
				as = std_str[i] != std_str[j];
				CHECK(a == as);

				a  = cpp_str[i] < cpp_str[j];
				as = std_str[i] < std_str[j];
				CHECK(a == as);

				a  = cpp_str[i] <= cpp_str[j];
				as = std_str[i] <= std_str[j];
				CHECK(a == as);

				a  = cpp_str[i] > cpp_str[j];
				as = std_str[i] > std_str[j];
				CHECK(a == as);

				a  = cpp_str[i] >= cpp_str[j];
				as = std_str[i] >= std_str[j];
				CHECK(a == as);
			}
		}
	}

	SECTION("Case 12")
	{
		String str("Mostafa");
		String other("Saad");
		str.concat(other);

		CHECK(str.size() == 12);
		CHECK(str.count() == 11);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 12);
		CHECK(str == "MostafaSaad");
	}

	SECTION("Case 13")
	{
		String str("Mostafa");
		str.concat("Saad");

		CHECK(str.size() == 12);
		CHECK(str.count() == 11);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 12);
		CHECK(str == "MostafaSaad");
	}

	SECTION("Case 14")
	{
		String str("Mostafa مصطفى 調ヨ沖掲不");
		String english = str.substr(0, 7);
		String arabic = str.substr(8, 13);
		String japanese = str.substr(14, 19);

		CHECK(english.count() == 7);
		CHECK(arabic.count() == 5);
		CHECK(japanese.count() == 5);
	}

	SECTION("Case 15")
	{
		String str("Mostafa");
		CHECK(str.size() == 8);
		CHECK(str.count() == 7);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 8);

		str.clear();
		CHECK(str.size() == 0);
		CHECK(str.count() == 0);
		CHECK(str.empty() == true);
		CHECK(str.capacity() >= 8);
	}

	SECTION("Case 16")
	{
		String str("Mostafa");
		CHECK(str.size() == 8);
		CHECK(str.count() == 7);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 8);

		str.reset();
		CHECK(str.size() == 0);
		CHECK(str.count() == 0);
		CHECK(str.empty() == true);
		CHECK(str.capacity() == 0);
	}

	SECTION("Case 17")
	{
		String str;
		CHECK(str.size() == 0);
		CHECK(str.count() == 0);
		CHECK(str.empty() == true);
		CHECK(str.capacity() == 0);

		str.reserve(8);
		CHECK(str.size() == 0);
		CHECK(str.count() == 0);
		CHECK(str.empty() == true);
		CHECK(str.capacity() == 8);
	}

	SECTION("Case 18")
	{
		const char* name = "Mostafa";
		String str(name);
		CHECK(str.size() == 8);
		CHECK(str.count() == 7);
		CHECK(str.empty() == false);
		CHECK(str.capacity() >= 8);

		auto str_range = str.all();
		CHECK(str == str_range);

		str_range = str.range(0, 7);
		CHECK(str == str_range);

		str_range = str.range(0, 6);
		CHECK(str != str_range);
	}
}