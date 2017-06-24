#include "catch.hpp"
#include <cpprelude/associative_array.h>
#include <cpprelude/dynamic_array.h>
#include<cpprelude/algorithm.h>

#include<string>
#include <functional>
#include <iostream>
using namespace cpprelude;


TEST_CASE("associative_array test", "[associative_array]")
{
	_RB_tree<int, int> array;
	dynamic_array<int> arr;
	std::function <void(const _RB_tree<int, int>::RB_Node&)> insert = [&arr](const _RB_tree<int, int>::RB_Node& obj) {
		arr.insert_back(obj.key);
	};

	std::function <void(const _RB_tree<int, int>::RB_Node&)> print = [](const _RB_tree<int, int>::RB_Node& obj) {
		auto c = obj.color == _color_type::RED ? "RED" : "BLACK";
		std::cout << "key: " << obj.key << " value: " << obj.value << " color: " << c << std::endl;
		auto temp = obj.parent != nullptr ? obj.parent->key : 0;
		std::cout << "parent " << temp << " ";
		temp = obj.left != nullptr ? obj.left->key : 0;
		std::cout << "left child " << temp << " ";
		temp = obj.right != nullptr ? obj.right->key : 0;
		std::cout << "right child " << temp << std::endl;
		std::cout << "==================================\n";
	};

	//SECTION("Case 01")
	//{
	//	//associative_array<int,int> array;
	//	

	//	array.insert(1, 11);
	//	CHECK(array.count()==1);
	//	//array.inorder_traverse(fun);
	//	array.insert(2, 22);
	//	CHECK(array.count() == 2);
	//	//array.inorder_traverse(fun);
	//	array.insert(3, 33);
	//	CHECK(array.count() == 3);
	//	array.insert(3, 44); 
	//	array.inorder_traverse(print);
	//	
	//}

	////Check right right case
	//SECTION("Case 02")
	//{
	//	array.insert(1, 11);
	//	CHECK(array.count() == 1);
	//	array.insert(2, 22);
	//	CHECK(array.count() == 2);
	//	array.insert(3, 33);
	//	CHECK(array.count() == 3);
	//	array.inorder_traverse(insert);
	//	CHECK(arr.count() == 3);
	//	CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	//}

	////Check left left case
	//SECTION("Case 03")
	//{
	//	array.insert(3, 33); 
	//	CHECK(array.count() == 1);
	//	array.insert(2, 22);
	//	CHECK(array.count() == 2);
	//	array.insert(1, 11);
	//	CHECK(array.count() == 3);
	//	array.inorder_traverse(insert);
	//	CHECK(arr.count() == 3);
	//	CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	//}

	////Check right left case
	//SECTION("Case 04")
	//{
	//	array.insert(1, 11);
	//	CHECK(array.count() == 1);
	//	array.insert(3, 33);
	//	CHECK(array.count() == 2);
	//	array.insert(2, 22);
	//	CHECK(array.count() == 3);
	//	array.inorder_traverse(insert);
	//	CHECK(arr.count() == 3);
	//	CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	//}

	////Check left right case
	//SECTION("Case 05")
	//{
	//	array.insert(3, 33);
	//	CHECK(array.count() == 1);
	//	array.insert(1, 11);
	//	CHECK(array.count() == 2);
	//	array.insert(2, 22);
	//	CHECK(array.count() == 3);
	//	array.inorder_traverse(insert);
	//	CHECK(arr.count() == 3);
	//	CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	//	//edit value
	//	array.insert(2, 44);
	//	CHECK(arr.count() == 3);
	//	CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	//}

	//SECTION("Case 06")
	//{
	//	array.insert(3, 33);
	//	CHECK(array.count() == 1);
	//	array.insert(1, 11);
	//	CHECK(array.count() == 2);
	//	array.insert(2, 22);
	//	CHECK(array.count() == 3);
	//	array.inorder_traverse(insert);
	//	CHECK(arr.count() == 3);
	//	CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	//	//edit value
	//	array[1] = 44;
	//	CHECK(array[3] == 33);
	//	//CHECK(array[0] == 33);
	//	//array[0] = 3;
	//	CHECK(arr.count() == 3);
	//	CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	//}

	SECTION("Case 07")
	{
		//using RB_Node = associative_array<int, int>::RB_Node;
		/*auto y = array.lookup(1);
		if (*y == nullptr)
		{
			*y._element = new associative_array<int, int>::RB_Node(0,3);
		}*/
		//array.inorder_traverse(print);

		array.insert(1, 11);
		array.insert(2, 22);
		array.insert(3, 33);
		auto x = array.insert(3, 22);
		//if (x == array._root)
			std::cout << (x)->key <<" " <<(x)->value<<std::endl;

	}
}
