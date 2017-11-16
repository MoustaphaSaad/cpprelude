#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/platform.h"

namespace cpprelude
{
	struct memory_watcher
	{
		usize _a_alive_allocations = 0, _b_alive_allocations = 0;
		usize _a_allocation_size = 0, _b_allocation_size = 0;
		const char* name = nullptr;

		API memory_watcher(const char* scope_name = nullptr);
		API ~memory_watcher();

		API void
		begin_watching();

		API void
		end_watching();

		API usize
		delta_alive_allocations() const;

		API usize
		delta_size() const;

		API void
		print_report() const;
	};
}