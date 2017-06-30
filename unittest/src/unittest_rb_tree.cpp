#include "catch.hpp"
#include <cpprelude/rb_tree.h>
#include <cpprelude/dynamic_array.h>
#include<cpprelude/algorithm.h>
#include<cpprelude/iterator.h>

#include<string>
#include <functional>
#include <iostream>
using namespace cpprelude;

template<typename T>
void print_fn(rb_tree_iterator<T> it)
{
	auto c = it->color == details::color_type::RED ? "RED" : "BLACK";
	std::cout << "key: " << it->data /*<< " value: " << obj.value*/ << " color: " << c << std::endl;
	auto temp = it->parent != nullptr ? it->parent->data : 0;
	std::cout << "parent " << temp << " ";
	temp = it->left != nullptr ? it->left->data : 0;
	std::cout << "left child " << temp << " ";
	temp = it->right != nullptr ? it->right->data : 0;
	std::cout << "right child " << temp << std::endl;
	std::cout << "==================================\n";
}

TEST_CASE("associative_array test", "[associative_array]")
{
	rb_tree<usize> array;
	dynamic_array<usize> arr;
	std::function <void(rb_tree_iterator<usize> it)> insert = [&arr](rb_tree_iterator<usize> it) {
		arr.insert_back(it->data);
	};

	
	std::function <void(rb_tree_iterator<usize> it)> print = [](rb_tree_iterator<usize> it) {
		auto c = it->color == details::color_type::RED ? "RED" : "BLACK";
		std::cout << "key: " << it->data /*<< " value: " << obj.value*/ << " color: " << c << std::endl;
		auto temp = it->parent != nullptr ? it->parent->data : 0;
		std::cout << "parent: " << temp << " ";
		temp = it->left != nullptr ? it->left->data : 0;
		std::cout << "left child: " << temp << " ";
		temp = it->right != nullptr ? it->right->data : 0;
		std::cout << "right child: " << temp << std::endl;
		std::cout << "==================================\n";
	};

	SECTION("Case 01")
	{
		array.insert(1);
		CHECK(array.count()==1);
		//array.inorder_traverse(fun);
		array.insert(2);
		CHECK(array.count() == 2);
		//array.inorder_traverse(fun);
		array.insert(3);
		CHECK(array.count() == 3);
		array.insert(3); 
		array.inorder_traverse(insert);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	////Check right right case
	SECTION("Case 02")
	{
		array.insert(1);
		CHECK(array.count() == 1);
		array.insert(2);
		CHECK(array.count() == 2);
		array.insert(3);
		CHECK(array.count() == 3);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//Check left left case
	SECTION("Case 03")
	{
		array.insert(3); 
		CHECK(array.count() == 1);
		array.insert(2);
		CHECK(array.count() == 2);
		array.insert(1);
		CHECK(array.count() == 3);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//Check right left case
	SECTION("Case 04")
	{
		array.insert(1);
		CHECK(array.count() == 1);
		array.insert(3);
		CHECK(array.count() == 2);
		array.insert(2);
		CHECK(array.count() == 3);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//Check left right case
	SECTION("Case 05")
	{
		array.insert(3);
		CHECK(array.count() == 1);
		array.insert(1);
		CHECK(array.count() == 2);
		array.insert(2);
		CHECK(array.count() == 3);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}
	
	//node type is pair 
	SECTION("Case 06")
	{
		using pair_node = details::pair_node<usize, std::string>;
		rb_tree<pair_node> tree;

		dynamic_array<pair_node> arr1;
		std::function <void(rb_tree_iterator<pair_node> it)> insert = [&arr1](rb_tree_iterator<pair_node> it) {
			arr1.insert_back(it->data.key);
		};

		tree.insert(pair_node(3, "33"));
		CHECK(tree.count() == 1);
		tree.insert(pair_node(1, "11"));
		CHECK(tree.count() == 2);
		tree.insert(pair_node(2, "22"));
		CHECK(tree.count() == 3);
		tree.inorder_traverse(insert);
		CHECK(arr1.count() == 3);
		CHECK(cpprelude::is_sorted(arr1.begin(), arr1.count()));
		CHECK(tree.is_rb_tree());
		//edit value
		auto x = pair_node(1);
		auto it = tree[x];
		CHECK(it->data == 1);
		CHECK(it->data.value == "11");
	}

	SECTION("Case 07")
	{
		rb_tree<usize> tree;

		usize i = 3;
		tree.insert(i);
		tree.insert(5);
		//tree.inorder_traverse(print);
		//Copy constructor
		rb_tree<usize> tree2(tree);
		tree2.inorder_traverse(insert);
		CHECK(tree2.count() == 2);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(tree2.is_rb_tree());
		tree2.insert(4);
		//Assignment operator
		tree = tree2;
		CHECK(tree.count() == 3);
		arr.reset();
		tree.inorder_traverse(insert);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(tree2.is_rb_tree());
	}

	SECTION("Case 08")
	{
		rb_tree<usize> array;
		array.insert(3);
		CHECK(array.count() == 1);
		array.insert(1);
		CHECK(array.count() == 2);
		array.insert(2);
		CHECK(array.count() == 3);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
		CHECK(arr.count() == 3);
		//search 
		CHECK(array[3]->data == 3);

		CHECK(array.lookup(99) == nullptr);
		auto res = array.lookup(1);
		bool f = res != nullptr && res->data == 1;
		CHECK(f);
	}

	SECTION("Case 09")
	{
		rb_tree<usize> array;
		array.insert(3);
		CHECK(array.count() == 1);
		array.insert(1);
		CHECK(array.count() == 2);
		array.insert(2);
		//auto fn = print_fn<usize>;
		//array.inorder_traverse(fn);
		CHECK(array.is_rb_tree());
	}
	
	//deleting root
	SECTION("Case 10")
	{
		rb_tree<usize> array;
		array.insert(3);
		CHECK(array.count() == 1);
		CHECK(array.is_rb_tree());

		//deleting non-existed element
		array.delete_rb_tree(0);
		CHECK(array.count() == 1);
		CHECK(array.is_rb_tree());

		//deleting root wuth no children
		array.delete_rb_tree(3);
		CHECK(array.count() == 0);
		CHECK(array.is_rb_tree());
		
	}
	
	//deleting root with one child
	SECTION("Case 11")
	{
		rb_tree<usize> array;
		
		//deleting root with left children
		array.insert(3);
		array.insert(1);
		CHECK(array.count() == 2);
		array.delete_rb_tree(3);
		CHECK(array.count() == 1);
		CHECK(array.get_root()->data == 1);
		//array.insert(3);
		//CHECK(array.count() == 2);
		CHECK(array.is_rb_tree());

		rb_tree<usize> array1;
		//deleting root with right children

		array1.insert(3);
		array1.insert(4);
		CHECK(array1.count() == 2);
		array1.delete_rb_tree(3);
		CHECK(array1.count() == 1);
		CHECK(array1.get_root()->data == 4);
		CHECK(array.is_rb_tree());
	}
	
	//deleting root with two children
	SECTION("Case 12")
	{
		rb_tree<usize> array;

		array.insert(3);
		array.insert(1);
		array.insert(6);
		CHECK(array.count() == 3);
		array.delete_rb_tree(3);
		CHECK(array.count() == 2);
		CHECK(array.get_root()->data == 6);
		array.insert(3);
		CHECK(array.count() == 3);
		CHECK(array.is_rb_tree());
	}
	
	//deleting node with two children ex 1
	//Case 3 & 4
	SECTION("Case 13")
	{
		rb_tree<usize> array;
		usize count = 0;
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == 8);
		//array.inorder_traverse(print);
		array.delete_rb_tree(15);
		CHECK(array.count() == 7);
		//array.inorder_traverse(print);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 7);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}
	
	//deleting node with two children ex 2
	//Case 3 & 4
	SECTION("Case 14")
	{
		rb_tree<usize> array;
		usize count = 0;
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.delete_rb_tree(15);
		CHECK(array.count() == --count);
		//array.inorder_traverse(print);
		array.inorder_traverse(insert);
		CHECK(arr.count() == count);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}
	
	//deleting node with one children ex 1
	SECTION("Case 15")
	{
		rb_tree<usize> array;
		usize count = 0;
		//deleting root with left children
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.delete_rb_tree(12);
		CHECK(array.count() == --count);
		//array.inorder_traverse(print);
		array.inorder_traverse(insert);
		CHECK(arr.count() == count);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}
	
	//deleting node with one children ex 2
	SECTION("Case 16")
	{
		rb_tree<usize> array;
		usize count = 0;
		//deleting root with left children
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.delete_rb_tree(20);
		CHECK(array.count() == --count);
		//array.inorder_traverse(print);
		array.inorder_traverse(insert);
		CHECK(arr.count() == count);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//insertion 
	SECTION("Case 17")
	{
		rb_tree<usize> tree;
		tree.insert(15);
		tree.insert(1);
		tree.insert(20);
		tree.insert(3);
		tree.insert(13);
		tree.insert(14);
		tree.insert(45);
		tree.insert(34);
		CHECK(tree.count() == 8);
		tree.inorder_traverse(insert);
		CHECK(arr.count() == 8);
		CHECK(tree.is_rb_tree());
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	}

	//Case 2 ex 1
	SECTION("Case 18")
	{
		rb_tree<usize> array;
		usize count = 0;
		//deleting root with left children
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.delete_rb_tree(20);
		CHECK(array.count() == --count);
		CHECK(array.is_rb_tree());
		array.delete_rb_tree(14);
		CHECK(array.count() == --count);
		CHECK(array.is_rb_tree());
		array.delete_rb_tree(12);
		CHECK(array.count() == --count);
		CHECK(array.is_rb_tree());
		//array.inorder_traverse(print);
	}

	
	SECTION("Case 19")
	{
		rb_tree<usize> array;
		usize count = 0;
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//Case 2 ex 2
		array.delete_rb_tree(20);
		CHECK(array.is_rb_tree());
		CHECK(array.count() == --count);
		array.delete_rb_tree(14);
		CHECK(array.is_rb_tree());
		CHECK(array.count() == --count);
		//Case 2
		array.delete_rb_tree(19);
		CHECK(array.is_rb_tree());
		CHECK(array.count() == --count);
		CHECK(array.is_rb_tree());
		array.delete_rb_tree(15);
		CHECK(array.is_rb_tree());
		CHECK(array.count() == --count);
		//Case 4
		array.delete_rb_tree(12);
		CHECK(array.is_rb_tree());
		CHECK(array.count() == --count);
	}
}
