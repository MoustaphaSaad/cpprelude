#include "cpprelude/memory_watcher.h"

namespace cpprelude
{
	//memory watcher
	memory_watcher_t::memory_watcher_t(const char* scope_name)
		:name(scope_name)
	{
		if(name == nullptr) name = "unnamed";
		begin_watching();
	}

	memory_watcher_t::~memory_watcher_t()
	{
		end_watching();
#ifdef DEBUG
		print_report();
#endif
	}

	void
	memory_watcher_t::begin_watching()
	{
		_a_alive_allocations = platform.allocation_count;
		_a_allocation_size = platform.allocation_size;
	}

	void
	memory_watcher_t::end_watching()
	{
		_b_alive_allocations = platform.allocation_count;
		_b_allocation_size = platform.allocation_size;
	}

	usize
	memory_watcher_t::delta_alive_allocations() const
	{
		return _b_alive_allocations - _a_alive_allocations;
	}

	usize
	memory_watcher_t::delta_size() const
	{
		return _b_allocation_size - _a_allocation_size;
	}

	void
	memory_watcher_t::print_report() const
	{
		if(delta_alive_allocations() != 0)
		{
			println(std::cout, "memory_watcher ", name,
					"{alive allocations: ", delta_alive_allocations(),
					", size: ", delta_size(), "}");
		}
	}
}