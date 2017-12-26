#include "benchmark.h"
#include <cpprelude/platform.h>
#include <cpprelude/string.h>
#include <cpprelude/allocator.h>
#include <cpprelude/hash_array.h>
#include <cpprelude/dynamic_array.h>
#include <cpprelude/stack_list.h>
#include <cpprelude/memory_context.h>
#include <cpprelude/memory_watcher.h>
#include <cpprelude/fmt.h>
using namespace cpprelude;

struct koko
{
	hash_array<usize, usize> d;

	koko(memory_context* context = platform->global_memory)
		:d(context)
	{}
};

struct A
{
	hash_array<usize, koko> data;

	A(memory_context* context = platform->global_memory)
		:data(context)
	{
		koko val(context);
		data.insert(1, val);
		data[0] = val;
	}
};

void
do_stuff()
{
	arena_t mem_arena(MEGABYTES(25));
	{
		stack_list<A> stuff(mem_arena);
		A a(mem_arena);
		stuff.push(a);
	}
}

void
do_other_stuff()
{
	string str;
	while (true)
	{
		scanln(str);
		if (str == "exit"_cs)
			break;
		println(str);
	}
}

int
main(int argc, char** argv)
{
	// do_stuff();
	do_other_stuff();
	return 0;
	do_benchmark();
	return 0;
}
