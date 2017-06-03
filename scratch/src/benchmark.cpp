#include "benchmark.h"
#include "stopwatch.h"

#include <cpprelude/dynamic_array.h>
#include <vector>

#include <cpprelude/stack_array.h>
#include <cpprelude/stack_list.h>
#include <stack>

#include <cpprelude/slinked_list.h>
#include <cpprelude/dlinked_list.h>
#include <list>
#include <deque>
#include <forward_list>

#include <cpprelude/queue_list.h>
#include <cpprelude/queue_array.h>
#include <queue>

#include <cpprelude/algorithm.h>
#include <algorithm>

#include <iostream>

void
benchmark_dynamic_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark dynamic_array" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_dynamic_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark dynamic_array custom allocator" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_slinked_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::slinked_list<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_front(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();

	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark slinked_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_slinked_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);
	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::slinked_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_front(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark custom slinked_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_dlinked_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dlinked_list<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark dlinked_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_dlinked_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);
	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dlinked_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark custom dlinked_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_stack_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::stack_array<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark stack_array" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_stack_array(cpprelude::usize limit)
{
	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;

	{
		cpprelude::stack_array<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push(i);
		w.stop();
	}

	std::cout << "benchmark custom stack_array" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_stack_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::stack_list<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark stack_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_stack_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::stack_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark custom stack_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_queue_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::queue_list<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.enqueue(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark queue_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_queue_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::queue_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.enqueue(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark custom queue_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_queue_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::queue_array<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.enqueue(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark queue_array" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_queue_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::queue_array<cpprelude::usize, 128, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.enqueue(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark custom queue_array" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_merge_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize> array;

		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(distribution(generator));

		w.start();
		cpprelude::merge_sort(array.begin(), array.count());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark merge_sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_quick_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize> array;

		for (cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(distribution(generator));

		w.start();
		cpprelude::quick_sort(array.begin(), array.count());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark quick_sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_insertion_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize> array;

		for (cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(distribution(generator));

		w.start();
		cpprelude::insertion_sort(array.begin(), array.count());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark insertion_sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

//STD

void
benchmark_vector(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::vector<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark vector" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_stack(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::stack<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark stack" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_list(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::list<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push_front(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_deque(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::deque<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();

	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark deque" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_forward_list(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::forward_list<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push_front(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark forward_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_queue(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::queue<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark queue" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_std_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::vector<cpprelude::usize> array;

		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push_back(distribution(generator));

		w.start();
		std::sort(array.begin(), array.end());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark std::sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_std_stable_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::vector<cpprelude::usize> array;

		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push_back(distribution(generator));

		w.start();
		std::stable_sort(array.begin(), array.end());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark std::stable_sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void 
benchmark()
{
	cpprelude::usize limit = 10000;

	std::cout << "\nBENCHMARK START\n" << std::endl;

	benchmark_vector(limit);
	std::cout << std::endl;
	benchmark_dynamic_array(limit);
	std::cout << std::endl;
	benchmark_custom_dynamic_array(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_stack(limit);
	std::cout << std::endl;
	benchmark_stack_array(limit);
	std::cout << std::endl;
	benchmark_stack_list(limit);
	std::cout << std::endl;
	benchmark_custom_stack_array(limit);
	std::cout << std::endl;
	benchmark_custom_stack_list(limit);

	std::cout <<"============================================================"<< std::endl;
	
	benchmark_forward_list(limit);
	std::cout << std::endl;
	benchmark_slinked_list(limit);
	std::cout << std::endl;
	benchmark_custom_slinked_list(limit);


	std::cout <<"============================================================"<< std::endl;

	benchmark_list(limit);
	std::cout << std::endl;
	benchmark_deque(limit);
	std::cout << std::endl;
	benchmark_dlinked_list(limit);
	std::cout << std::endl;
	benchmark_custom_dlinked_list(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_queue(limit);
	std::cout << std::endl;
	benchmark_queue_list(limit);
	std::cout << std::endl;
	benchmark_custom_queue_list(limit);
	std::cout << std::endl;
	benchmark_queue_array(limit);
	std::cout << std::endl;
	benchmark_custom_queue_array(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_std_sort(limit);
	std::cout << std::endl;
	benchmark_std_stable_sort(limit);
	std::cout << std::endl;
	benchmark_merge_sort(limit);
	std::cout << std::endl;
	benchmark_quick_sort(limit);
	std::cout << "\nnext size = 1/10 of original benchmark size\n" << std::endl;
	benchmark_insertion_sort(limit/10);

	std::cout << "\nBENCHMARK END\n" << std::endl;
}
