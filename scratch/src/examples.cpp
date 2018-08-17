#include "examples.h"

#define BLOCK_WARNING(var) ((void)var)

/**
 * Example 1
 * we are just doing a hello world example using the basic types
 */
#include <cpprelude/IO.h>
using namespace cppr;

void
e1_hello_world()
{
	println("Hello, World!");
	printfmt("Hello, My name is {}, and my age is {}\n", "Mostafa", 25);
	println_err("This is a print into the error buffer");
	printfmt_err("Hello, I'm error number {}\n", -1);
}


/**
 * Example 2
 * we are just demonstrating the basic types
 */
#include <cpprelude/defines.h>
using namespace cppr;

void
e2_basic_types()
{
	byte 		_a;
	ubyte 		_b;		//unsigned byte
	u8 			_c;		//unsigned 8 bit integer
	i8			_d;		//signed 8 bit integer
	u16			_e;		//unsigned 16 bit integer
	i16			_f;		//signed 16 bit integer
	u32			_g;		//unsigned 32 bit integer
	i32			_h;		//signed 32 bit integer
	u64			_i;		//unsigned 64 bit integer
	i64			_j;		//signed 64 bit integer
	usize		_k;		//unsigned integer to represent size (u32 in case of x86, u64 in case of x64)
	isize		_l;		//signed integer to represent size (u32 in case of x86, u64 in case of x64)
	r32			_m;		//real 32 bit number
	r64			_n;		//real 64 bit number

	BLOCK_WARNING(_a);
	BLOCK_WARNING(_b);
	BLOCK_WARNING(_c);
	BLOCK_WARNING(_d);
	BLOCK_WARNING(_e);
	BLOCK_WARNING(_f);
	BLOCK_WARNING(_g);
	BLOCK_WARNING(_h);
	BLOCK_WARNING(_i);
	BLOCK_WARNING(_j);
	BLOCK_WARNING(_k);
	BLOCK_WARNING(_l);
	BLOCK_WARNING(_m);
	BLOCK_WARNING(_n);
}


/**
 * Example 3
 * we will create a simple calculator
 */

#include <cpprelude/IO.h>
#include <cpprelude/Panic.h>
using namespace cppr;

void
e3_calculator()
{
	i32 a, b;
	Rune c;				//Rune is a single utf8 letter
	println("please enter add or sub operation i.e: \"1+2\"");
	read(a, c, b);		//reads from the stdin a number `a` then Rune `c` then number `b`
	switch(c.data)
	{
		case '+':
			println(a + b);
			break;

		case '-':
			println(a - b);
			break;

		default:
			panic("Illegal operator");
			break;
	}
}


/**
 * Example 4
 * we will allocate some data on the heap
 */

#include <cpprelude/OS.h>
using namespace cppr;

struct Base
{};

struct Child: Base
{
	Child(int x){}
};

void
e4_heap()
{
	//alloc a single i32
	Owner<i32> my_number = alloc<i32>();
	//alloc 10 i32
	Owner<i32> my_array = alloc<i32>(10);
	//note that alloc doesn't invoke any constructors

	//construct a single `1` Child and invoke the constructor with the argument `35`
	//and store it in Base (this invokes constructors)
	Owner<Base> ptr = construct<Child>(1, 35);

	//you can access the underlying pointer
	i32* arr_begin = my_array.ptr;
	BLOCK_WARNING(arr_begin);

	//you can access the size in bytes
	usize arr_size_in_bytes = my_array.size;
	BLOCK_WARNING(arr_size_in_bytes);

	//you can access the count of elements
	usize arr_count = my_array.count();
	BLOCK_WARNING(arr_count);

	//don't forget to free the allocated memory
	free(my_number);
	free(my_array);
	//note that free doesn't invoke any destructors

	//destruct the ptr (this invokes destructors)
	destruct(ptr);
}

/**
 * Example 5
 * we will use Dynamic Array type
 */

#include <cpprelude/IO.h>
#include <cpprelude/Dynamic_Array.h>
#include <cpprelude/Ranges.h>
#include <cpprelude/Algorithms.h>
#include <numeric>
using namespace cppr;

void
e5_dynamic_array()
{
	Dynamic_Array<i32> numbers(1000);
	for(auto& num: numbers)
		num = rand() % 100;

	i64 acc = std::accumulate(numbers.begin(), numbers.end(), 0);
	printfmt("avg is {:.2f}\n", r32(acc) / r32(numbers.count()));

	//A slice is just a range(view) over the underlying dynamic array
	//so if you change an element in the slice it will be changed in the underlying dynamic array
	Slice<i32> first_half = numbers.range(0, 500);
	Slice<i32> second_half = numbers.range(500, 1000);

	acc = std::accumulate(first_half.begin(), first_half.end(), 0);
	printfmt("first half avg {:.2f}\n", r32(acc) / r32(first_half.count()));

	acc = std::accumulate(second_half.begin(), second_half.end(), 0);
	printfmt("second half avg {:.2f}\n", r32(acc) / r32(second_half.count()));

	quick_sort(first_half);
	printfmt("first half median {}\n", first_half[500/2]);

	merge_sort(second_half);
	printfmt("second half median {}\n", second_half[500/2]);
}


/**
 * Example 6
 * we will use the String type
 */

#include <cpprelude/IO.h>
#include <cpprelude/String.h>

void
e6_string()
{
	//A String is a utf8 string
	String en_name = u8"Mostafa Saad";
	String ar_name = u8"مصطفى سعد";
	println(en_name);
	
	String_Range first_name = en_name.range(0, 7);
	String_Range second_name = en_name.range(8, 12);
	printfmt("first name: ({}), second_name: ({})\n", first_name, second_name);

	first_name = ar_name.range(0, 5);
	second_name = ar_name.range(6, 9);
	printfmt("first name: ({}), second_name: ({})\n", first_name, second_name);

	//A Rune is a utf8 letter/char
	for(Rune r: en_name)
		println(r);

	for(Rune r: ar_name)
		println(r);

	en_name.concat(u8" Abdel-Hameed ");
	en_name.concat(ar_name.range(0, 5));
	println(en_name);

	const char* c_str = en_name.data();
	usize c_str_size = en_name.size();
	usize rune_count = en_name.count();

	BLOCK_WARNING(c_str);
	BLOCK_WARNING(c_str_size);
	BLOCK_WARNING(rune_count);

	//read user name
	String user_name;
	println("please enter your name");
	read(user_name);
	println("your name is ", user_name);
}

/**
 * Example 7
 * we will use the hash map type through calculating the frequency of some words
 */
#include <cpprelude/IO.h>
#include <cpprelude/Hash_Map.h>
#include <cpprelude/Tree_Map.h>
using namespace cppr;

void
e7_hash_map()
{
	Hash_Map<String, i32> dict;
	Tree_Map<String, i32> ordered_dict;
	Dynamic_Array<String> words;

	String content = u8"the quick brown fox jumps over the lazy dog";

	String_Iterator tmp = content.begin();
	for(auto it = content.begin();
		it != content.end();
		++it)
	{
		if(*it == ' ')
		{
			words.emplace_back(content.substr(tmp, it));
			tmp = it;
			++tmp;
		}
	}

	for(auto word: words)
	{
		dict[word]++;
		ordered_dict[word]++;
	}

	 for(const auto& entry: dict)
	 	printfmt("dict[{:10}]: {:10}\n", entry.key, entry.value);

	for(const auto& entry: ordered_dict)
	 	printfmt("ordered_dict[{:10}]: {:10}\n", entry.key, entry.value);
}

/**
 * Example 8
 * we will use the custom allocators with a single linked list
 */
#include <cpprelude/IO.h>
#include <cpprelude/Single_List.h>
#include <cpprelude/Allocators.h>
using namespace cppr;

void
e8_custom_allocator()
{
	Arena_Allocator numbers_arena;
	Single_List<usize> my_numbers(numbers_arena);
	for(usize i = 0; i < 1000; ++i)
		my_numbers.insert_front(i);

	printfmt("used memory: {}, free memory: {}\n",
		numbers_arena.used_memory_size(),
		numbers_arena.unused_memory_size());
}


/**
 * Example 9
 * we will use the custom allocators with a single linked list
 */
#include <cpprelude/IO.h>
#include <cpprelude/File.h>
#include <cpprelude/Buffered_Stream.h>
#include <cpprelude/String.h>
using namespace cppr;

void
e9_load_file()
{
	//you can use the unwrap function
	File file = unwrap(File::open("my_file", IO_MODE::READ, OPEN_MODE::OPEN_ONLY));

	printfmt("file size: {}\n", file.size());

	//start reading from the file
	Buf_Reader reader(file);

	//read the file line by line
	String line;
	while(readln(reader, line))
		println(line);


	//we can ignore the error value
	File other_file = File::open("my_other_file", IO_MODE::READ, OPEN_MODE::OPEN_ONLY).value;

	Owner<byte> file_content = alloc<byte>(other_file.size());
	other_file.read(file_content.all());
	String content(std::move(file_content));
	println(content);
}

void
run_examples()
{
	e1_hello_world();
	e2_basic_types();
	e3_calculator();
	e4_heap();
	e5_dynamic_array();
	e6_string();
	e7_hash_map();
	e8_custom_allocator();
	e9_load_file();
}