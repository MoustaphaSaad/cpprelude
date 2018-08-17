//#include <vld.h>
#include "benchmark.h"

#include <cpprelude/Allocators.h>

#include <cpprelude/Dynamic_Array.h>
#include <vector>

#include <forward_list>
#include <cpprelude/Single_List.h>

#include <list>
#include <cpprelude/Double_List.h>

#include <cpprelude/Stack_Array.h>
#include <cpprelude/Stack_List.h>
#include <stack>

#include <cpprelude/Ring_Array.h>
#include <cpprelude/Queue_List.h>
#include <cpprelude/Priority_Queue.h>
#include <queue>

#include <cpprelude/Algorithms.h>
#include <algorithm>

#include <cpprelude/Tree_Map.h>
#include <map>

#include <cpprelude/String.h>
#include <string>

#include <cpprelude/Hash_Map.h>
#include <unordered_set>
#include <unordered_map>

#include <cpprelude/IO.h>

#include <cpprelude/Panic.h>
#include <cpprelude/Memory_Stream.h>
#include <cpprelude/Buffered_Stream.h>
#include <cpprelude/File.h>
#include <cpprelude/Benchmark.h>

#include <cpprelude/Loom.h>

using namespace cppr;

Dynamic_Array<usize> RANDOM_ARRAY;

void
generate_random_data(usize limit)
{
	srand(time(0));
	RANDOM_ARRAY.clear();
	for(usize i = 0; i < limit; ++i)
		RANDOM_ARRAY.insert_back(rand());
}


usize
bm_Dynamic_Array(Stopwatch& watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Dynamic_Array<usize> array;

		for(usize i = 0; i < limit; ++i)
			array.insert_back(i + r);

		for(const auto& number: array)
			if(number % 3 == 0)
				r += number;
	}
	watch.stop();

	return r;
}

usize
bm_vector(Stopwatch& watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::vector<usize> array;

		for(usize i = 0; i < limit; ++i)
			array.push_back(i + r);

		for (const auto& number : array)
			if (number % 3 == 0)
				r += number;
	}
	watch.stop();

	return r;
}


usize
bm_Single_List(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Single_List<usize> list;
		for(usize i = 0; i < limit; ++i)
			list.insert_front(i + r);

		for (const auto& number : list)
			if (number % 2 == 0)
				r += number;
	}
	watch.stop();
	return r;
}

usize
bm_Stack_Allocator_Single_List(Stopwatch &watch, usize limit)
{
	Stack_Allocator stack(MEGABYTES(250), os->virtual_memory);
	usize r = rand();

	stack.free_all();
	watch.start();
	{
		Single_List<usize> list(stack);
		for(usize i = 0; i < limit; ++i)
			list.insert_front(i + r);

		for (const auto& number : list)
			if (number % 2 == 0)
				r += number;
	}
	watch.stop();
	return r;
}

usize
bm_Arena_Allocator_Single_List(Stopwatch &watch, usize limit)
{
	Arena_Allocator arena;
	usize r = rand();

	watch.start();
	{
		Single_List<usize> list(arena);
		for(usize i = 0; i < limit; ++i)
			list.insert_front(i + r);

		for (const auto& number : list)
			if (number % 2 == 0)
				r += number;
	}
	watch.stop();

	arena.reset();

	return r;
}

usize
bm_forward_list(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::forward_list<usize> list;
		for(usize i = 0; i < limit; ++i)
			list.push_front(i + r);

		for(auto& number: list)
			if(number % 2 == 0)
				r += number;
	}
	watch.stop();
	return r;
}

usize
bm_Double_List(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Double_List<usize> list;
		for(usize i = 0; i < limit; ++i)
			if((i + r) % 2 == 0)
				list.emplace_back(i + r);
			else
				list.emplace_front(i + r);

		for(const auto& number: list)
			if(number % 2 == 0)
				r += number;
	}
	watch.stop();
	return r;
}


usize
bm_list(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::list<usize> list;
		for(usize i = 0; i < limit; ++i)
			if((i + r) % 2 == 0)
				list.push_back(i + r);
			else
				list.push_front(i + r);

		for(const auto& number: list)
			if(number % 2 == 0)
				r += number;
	}
	watch.stop();
	return r;
}


usize
bm_Stack_Array(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Stack_Array<usize> stack;
		for (usize i = 0; i < limit; ++i)
			if ((i + r) % 2 == 0)
				stack.push(i + r);

		while (!stack.empty())
		{
			r += stack.top();
			stack.pop();
		}
	}
	watch.stop();
	return r;
}

usize
bm_Stack_List(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Stack_List<usize> stack;
		for (usize i = 0; i < limit; ++i)
			if ((i + r) % 2 == 0)
				stack.push(i + r);

		while (!stack.empty())
		{
			r += stack.top();
			stack.pop();
		}
	}
	watch.stop();
	return r;
}

usize
bm_stack(Stopwatch &watch, usize limit)
{
	usize r = rand();
	watch.start();
	{
		std::stack<usize> stack;
		for (usize i = 0; i < limit; ++i)
			if ((i + r) % 2 == 0)
				stack.push(i + r);

		while (!stack.empty())
		{
			r += stack.top();
			stack.pop();
		}
	}
	watch.stop();
	return r;
}


usize
bm_Ring_Array(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Ring_Array<usize> que;
		que.reserve(limit);
		for (usize i = 0; i < limit; ++i)
			que.insert_front(i + r);

		while(!que.empty())
		{
			r += que.front();
			que.remove_front();
		}
	}
	watch.stop();
	return r;
}

usize
bm_Queue_List(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Queue_List<usize> que;
		for (usize i = 0; i < limit; ++i)
			que.enqueue(i + r);

		while (!que.empty())
		{
			r += que.front();
			que.dequeue();
		}
	}
	watch.stop();
	return r;
}

usize
bm_queue(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::queue<usize> que;
		for (usize i = 0; i < limit; ++i)
			que.push(i + r);

		while (!que.empty())
		{
			r += que.front();
			que.pop();
		}
	}
	watch.stop();
	return r;
}


usize
bm_Priority_Queue(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Priority_Queue<usize> array;
		for(usize i = 0; i < limit; ++i)
			array.enqueue(i + r);

		while(!array.empty())
		{
			if(array.front() % 2 == 0)
				r += array.front();
			array.dequeue();
		}
	}
	watch.stop();
	return r;
}

usize
bm_priority_queue(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::priority_queue<usize> array;
		for(usize i = 0; i < limit; ++i)
			array.push(i + r);

		while(!array.empty())
		{
			if(array.top() % 2 == 0)
				r += array.top();
			array.pop();
		}
	}
	watch.stop();
	return r;
}


usize
bm_Tree_Map(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Tree_Map<usize, usize> map;
		for(usize i = 0; i < limit; ++i)
			map[i] = i+r;
		for(usize i = 0; i < limit; ++i)
		{
			auto it = map.lookup(i);
			if(it != map.end())
			{
				if((it->key + it->value) % 2 == 0)
					map.remove(it);
			}
		}
	}
	watch.stop();

	return r;
}

usize
bm_map(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::map<usize, usize> map;
		for(usize i = 0; i < limit; ++i)
			map[i] = i+r;
		for(usize i = 0; i < limit; ++i)
		{
			auto it = map.find(i);
			if(it != map.end())
			{
				if((it->first + it->second) % 2 == 0)
					map.erase(it);
			}
		}
	}
	watch.stop();

	return r;
}


usize
bm_Hash_Set(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Hash_Set<usize> s;
		for(usize i = 0; i < limit; ++i)
			s.insert(r + i);
		for(usize i = r; i < limit + r; ++i)
			if(i % 2 == 0)
				s.remove(i);
		for(const auto& num: s)
			r += num;
	}
	watch.stop();
	return r;
}

usize
bm_Stack_Allocator_Hash_Set(Stopwatch &watch, usize limit)
{
	Stack_Allocator stack(MEGABYTES(250), os->virtual_memory);
	usize r = rand();

	stack.free_all();
	watch.start();
	{
		Hash_Set<usize> s(stack);
		for(usize i = 0; i < limit; ++i)
			s.insert(r + i);
		for(usize i = r; i < limit + r; ++i)
			if(i % 2 == 0)
				s.remove(i);
		for(const auto& num: s)
			r += num;
	}
	watch.stop();
	return r;
}

usize
bm_Arena_Allocator_Hash_Set(Stopwatch &watch, usize limit)
{
	Arena_Allocator arena;
	usize r = rand();

	watch.start();
	{
		Hash_Set<usize> s(arena);
		for(usize i = 0; i < limit; ++i)
			s.insert(r + i);
		for(usize i = r; i < limit + r; ++i)
			if(i % 2 == 0)
				s.remove(i);
		for(const auto& num: s)
			r += num;
	}
	watch.stop();

	arena.reset();
	return r;
}

usize
bm_unordered_set(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::unordered_set<usize> s;
		for(usize i = 0; i < limit; ++i)
			s.insert(r + i);
		for(usize i = r; i < limit + r; ++i)
			if(i % 2 == 0)
				s.erase(i);
		for(const auto& num: s)
			r += num;
	}
	watch.stop();
	return r;
}


usize
bm_Hash_Map(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Hash_Map<usize, usize> m;
		for (usize i = 0; i < limit; ++i)
			m[i] = r + i;
		for (usize i = 0; i < limit; ++i)
		{
			auto it = m.lookup(i);
			if (it->value % 2 == 0)
				m.remove(it);
		}
		for (const auto& it : m)
			r += it.value;
	}
	watch.stop();
	return r;
}

usize
bm_unordered_map(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::unordered_map<usize, usize> m;
		for(usize i = 0; i < limit; ++i)
			m[i] = r + i;
		for(usize i = 0; i < limit; ++i)
		{
			auto it = m.find(i);
			if(it->second % 2 == 0)
				m.erase(it);
		}
		for (const auto& it : m)
			r += it.second;
	}
	watch.stop();
	return r;
}


void
bm_String(Stopwatch &watch, usize limit)
{
	watch.start();
	{
		String str;
		for(usize i = 0; i < limit; ++i)
			str.concat("Mostafa");
	}
	watch.stop();
}

void
bm_string(Stopwatch &watch, usize limit)
{
	watch.start();
	{
		std::string str;
		for(usize i = 0; i < limit; ++i)
			str += "Mostafa";
	}
	watch.stop();
}


bool
bm_heap_sort(Stopwatch &watch, usize limit)
{
	Dynamic_Array<usize> array = RANDOM_ARRAY;

	watch.start();
		cppr::heap_sort(array.all());
	watch.stop();

	bool result = cppr::is_sorted(array.all());
	assert(result);

	return result;
}

bool
bm_std_heap_sort(Stopwatch &watch, usize limit)
{
	std::vector<usize> array(RANDOM_ARRAY.begin(), RANDOM_ARRAY.end());

	watch.start();
		std::make_heap(array.begin(), array.end());
		std::sort_heap(array.begin(), array.end());
	watch.stop();

	bool result = std::is_sorted(array.begin(), array.end());
	assert(result);

	return result;
}


bool
bm_merge_sort(Stopwatch &watch, usize limit)
{
	Dynamic_Array<usize> array = RANDOM_ARRAY;

	watch.start();
		cppr::merge_sort(array.all());
	watch.stop();

	bool result = cppr::is_sorted(array.all());
	assert(result);

	return result;
}

bool
bm_std_merge_sort(Stopwatch &watch, usize limit)
{
	std::vector<usize> array(RANDOM_ARRAY.begin(), RANDOM_ARRAY.end());

	watch.start();
		std::stable_sort(array.begin(), array.end());
	watch.stop();

	bool result = std::is_sorted(array.begin(), array.end());
	assert(result);

	return result;
}


bool
bm_quick_sort(Stopwatch &watch, usize limit)
{
	Dynamic_Array<usize> array = RANDOM_ARRAY;

	watch.start();
		cppr::quick_sort(array.all());
	watch.stop();

	bool result = cppr::is_sorted(array.all());
	assert(result);

	return result;
}

bool
bm_std_quick_sort(Stopwatch &watch, usize limit)
{
	std::vector<usize> array(RANDOM_ARRAY.begin(), RANDOM_ARRAY.end());

	watch.start();
		std::sort(array.begin(), array.end());
	watch.stop();

	bool result = std::is_sorted(array.begin(), array.end());
	assert(result);

	return result;
}

struct Screamer
{
	Screamer()
	{
		println("Screamer()");
	}

	Screamer(const Screamer& other)
	{
		println("Screamer(const Screamer&)");
	}

	Screamer(Screamer&& other)
	{
		println("Screamer(Screamer&&)");
	}

	Screamer&
	operator=(const Screamer& other)
	{
		println("operator=(const Screamer&)");
		return *this;
	}

	Screamer&
	operator=(Screamer&& other)
	{
		println("operator=(Screamer&&");
		return *this;
	}

	~Screamer()
	{
		println("~Screamer()");
	}
};

int x;

Executer*
task_wait(Executer* exe, Task::Arg arg)
{
	++x;
	//cppr::printf("Task #{}, From Worker #{}\n", exe->task.id, exe->worker.id);
	Stopwatch w;
	w.start();

	exe = exe->yield([](Task::Arg){
		return true;
	}, nullptr);

	w.stop();
	cppr::printfmt("task {} took {} microseconds\n", exe->task_desc(), w.microseconds());
	++x;
	return exe;
}

Executer*
task(Executer* exe, Task::Arg arg)
{
	// ++x;
	// usize msleep = usize(arg);
	// cppr::printf("Task #{}, From Worker #{}\n", exe->task.id, exe->worker.id);
	// auto id = exe->worker.id;
	// exe = exe->yield();
	// if(id != exe->worker.id)
	// cppr::printf("Back Task #{}, From Worker #{}\n", exe->task.id, exe->worker.id);
	// ++x;
	return exe;
}

Executer*
task_sender(Executer* exe, Task::Arg arg)
{
	Channel<int>* c = (Channel<int>*)arg;
	auto err = c->send(exe, -1);
	if(err)
		println("ERROR");
	return exe;
}

Executer*
task_reciever(Executer* exe, Task::Arg arg)
{
	Channel<int>* c = (Channel<int>*)arg;
	int x;
	auto err = c->recieve(exe, x);
	if(err)
		println("ERROR");
	return exe;
}

void
loom_debug()
{
	x = 0;
	Loom l;
	Owner<byte> mem;
	u32 max_tasks = 10000;
	u32 workers_count = 8;
	u32 max_fibers = 1000;
	u32 stack_size = KILOBYTES(32);
	usize required_size = l.init(std::move(mem), workers_count, max_tasks,
								 max_fibers, stack_size);
	mem = alloc<byte>(required_size);
	println(required_size/(1024.0f*1024.0f), "Mb");
	l.init(std::move(mem), workers_count, max_tasks, max_fibers, stack_size);

	Stopwatch watch;
	Channel<int> c(100);
	int acc = 0;
	
	watch.start();

	for(usize i = 0; i < max_fibers; ++i)
	{
		l.task_push("sender", task_sender, &c);
		l.task_push("reciever", task_reciever, &c);
	}

	/*for(usize i = 0; i < 20; ++i)
	{
		if(i < workers_count)
			l.task_push_to(Worker_Handle { u32(i) }, task, Task::Arg(i));
		else
			l.task_push(task, Task::Arg(i));
	}*/


	 {
	 	//l.task_push("long", task_wait, nullptr);

	 	/*for(usize i = 0; i < max_tasks; ++i)
	 		l.task_push(task, nullptr);

	 	for(usize i = 0; i < max_tasks; ++i)
	 		l.task_push(task, nullptr);*/
	 }

	l.wait_until_finished();
	watch.stop();

	cppr::printfmt("total: {}ms\n{}ns per routine\n", watch.milliseconds(), watch.nanoseconds() / (max_fibers));
	println(x);
	l.dispose();
	free(l.memory);
}

void
debug()
{
	allocator_push(os->leak_detector);
	(alloc<byte>(1024));
	(alloc<byte>(1022));
	allocator_pop();
}

void
do_benchmark()
{
	//allocator_push(os->leak_detector);
	// while(true)
	// {
	// 	loom_debug();
	// }
	debug();
	usize limit = 1000;

	compare_benchmarks(
		summary("std::vector"_rng, [&](Stopwatch& watch)
		{
			bm_vector(watch, limit);
		}),

		summary("Dynamic_Array"_rng, [&](Stopwatch& watch)
		{
			bm_Dynamic_Array(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::string"_rng, [&](Stopwatch& watch)
		{
			bm_string(watch, limit);
		}),

		summary("String"_rng, [&](Stopwatch& watch)
		{
			bm_String(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::forward_list"_rng, [&](Stopwatch& watch)
		{
			bm_forward_list(watch, limit);
		}),

		summary("Single_List"_rng, [&](Stopwatch& watch)
		{
			bm_Single_List(watch, limit);
		}),

		summary("Single_List Stack_Allocator"_rng, [&](Stopwatch& watch)
		{
			bm_Stack_Allocator_Single_List(watch, limit);
		}),

		summary("Single_List Arena_Allocator"_rng, [&](Stopwatch& watch)
		{
			bm_Arena_Allocator_Single_List(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::list"_rng, [&](Stopwatch& watch)
		{
			bm_list(watch, limit);
		}),

		summary("Double_List"_rng, [&](Stopwatch& watch)
		{
			bm_Double_List(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::stack"_rng, [&](Stopwatch& watch)
		{
			bm_stack(watch, limit);
		}),

		summary("Stack_Array"_rng, [&](Stopwatch& watch)
		{
			bm_Stack_Array(watch, limit);
		}),

		summary("Stack_List"_rng, [&](Stopwatch& watch)
		{
			bm_Stack_List(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::queue"_rng, [&](Stopwatch& watch)
		{
			bm_queue(watch, limit);
		}),

		summary("Ring_Array"_rng, [&](Stopwatch& watch)
		{
			bm_Ring_Array(watch, limit);
		}),

		summary("Queue_List"_rng, [&](Stopwatch& watch)
		{
			bm_Queue_List(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::priority_queue"_rng, [&](Stopwatch& watch)
		{
			bm_priority_queue(watch, limit);
		}),

		summary("Priority_Queue"_rng, [&](Stopwatch& watch)
		{
			bm_Priority_Queue(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::map"_rng, [&](Stopwatch& watch)
		{
			bm_map(watch, limit);
		}),

		summary("Tree_Map"_rng, [&](Stopwatch& watch)
		{
			bm_Tree_Map(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::unordered_set"_rng, [&](Stopwatch& watch)
		{
			bm_unordered_set(watch, limit);
		}),

		summary("Hash_Set"_rng, [&](Stopwatch& watch)
		{
			bm_Hash_Set(watch, limit);
		}),

		summary("Hash_Set Stack_Allocator"_rng, [&](Stopwatch& watch)
		{
			bm_Stack_Allocator_Hash_Set(watch, limit);
		}),

		summary("Hash_Set Arena_Allocator"_rng, [&](Stopwatch& watch)
		{
			bm_Arena_Allocator_Hash_Set(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::unordered_map"_rng, [&](Stopwatch& watch)
		{
			bm_unordered_map(watch, limit);
		}),

		summary("Hash_Map"_rng, [&](Stopwatch& watch)
		{
			bm_Hash_Map(watch, limit);
		})
	);

	generate_random_data(limit);

	println();

	compare_benchmarks(
		summary("std::heap_sort"_rng, [&](Stopwatch& watch)
		{
			bm_std_heap_sort(watch, limit);
		}),

		summary("heap_sort"_rng, [&](Stopwatch& watch)
		{
			bm_heap_sort(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::stable_sort"_rng, [&](Stopwatch& watch)
		{
			bm_std_merge_sort(watch, limit);
		}),

		summary("merge_sort"_rng, [&](Stopwatch& watch)
		{
			bm_merge_sort(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::sort"_rng, [&](Stopwatch& watch)
		{
			bm_std_quick_sort(watch, limit);
		}),

		summary("quick_sort"_rng, [&](Stopwatch& watch)
		{
			bm_quick_sort(watch, limit);
		})
	);
}
