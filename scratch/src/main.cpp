#include "benchmark.h"
#include <cpprelude/platform.h>
#include <cpprelude/string.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	auto handle = platform.open_file("mostafa.file"_cs);
	return 0;
	do_benchmark();
	return 0;
}
