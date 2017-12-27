 #include "catch.hpp"
 #include <cpprelude/string.h>
#include <string>

 using namespace cpprelude;

 TEST_CASE("string test", "[string]")
 {
 	string str;

 	SECTION("Case 01")
 	{
 		string new_str;

 		CHECK(str == new_str);
 		CHECK(str.empty() == true);
 		CHECK(new_str.empty() == true);
 		CHECK(str.count() == 0);
 		CHECK(new_str.count() == 0);
 	}

 	SECTION("Case 02")
 	{
 		string a(u8"mostafa");
 		string b;

 		for(auto c: b)
 			CHECK(false);

 		CHECK(a.empty() == false);
 		CHECK(a.count() == 7);

 		b = a;
		CHECK(b.empty() == false);
 		CHECK(b.count() == 7);

 		CHECK(a == b);

 		auto b_it = b.begin();
 		auto a_it = a.begin();
 		while(b_it != b.end())
 			CHECK(*b_it++ == *a_it++);
 	}

 	SECTION("Case 03")
 	{
 		string a(u8"مصطفى");
 		string b;

 		for(auto c: b)
 			CHECK(false);

 		CHECK(a.empty() == false);
 		CHECK(a.count() == 5);

 		b = a;
 		CHECK(b.empty() == false);
 		CHECK(b.count() == 5);

 		CHECK(a == b);

 		auto b_it = b.begin();
 		auto a_it = a.begin();
 		while(b_it != b.end())
 			CHECK(*b_it++ == *a_it++);
 	}

 	SECTION("Case 04")
 	{
 		string cpp_str[6] = {"abcd", "ab", "ba", "dcba", "fegh", "cdab"};
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

 	SECTION("Case 05")
 	{
 		string original = "Mostafa مصطفى 調ヨ沖掲不";
 		string english = original.substr(0, 7);
 		string arabic = original.substr(8, 5);
 		string japanese = original.substr(14, 5);

 		CHECK(english.count() == 7);
 		CHECK(arabic.count() == 5);
 		CHECK(japanese.count() == 5);

 		string en_view = original.view(0, 7);
 		string ar_view = original.view(8, 5);
 		string jp_view = original.view(14, 5);

 		CHECK(en_view.count() == 7);
 		CHECK(ar_view.count() == 5);
 		CHECK(jp_view.count() == 5);

 		CHECK(english == en_view);
 		CHECK(arabic == ar_view);
 		CHECK(japanese == jp_view);
 	}

 	SECTION("Case 06")
 	{
 		string str;
 		str.concat("my "_cs);
 		str.concat("name is "_cs);
 		str.concat("مصطفى"_cs);

 		CHECK(str == "my name is مصطفى");
 	}
 }
