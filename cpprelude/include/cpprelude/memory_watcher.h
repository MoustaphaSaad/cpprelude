#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"

namespace cpprelude
{
	struct memory_watcher
	{
		usize _a_alive_allocations = 0, _b_alive_allocations = 0;
		usize _a_allocation_size = 0, _b_allocation_size = 0;
		const char* name = nullptr;

		API_CPPR memory_watcher(const char* scope_name = nullptr);
		API_CPPR ~memory_watcher();

		API_CPPR void
		begin_watching();

		API_CPPR void
		end_watching();

		API_CPPR usize
		delta_alive_allocations() const;

		API_CPPR usize
		delta_size() const;

		API_CPPR void
		print_report() const;
	};
}