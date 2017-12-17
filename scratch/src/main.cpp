#include "benchmark.h"
#include <cpprelude/platform.h>
#include <cpprelude/string.h>
#include <cpprelude/file.h>
using namespace cpprelude;

void write()
{
	auto str = "KOKO"_cs;
	file my_file = file::open("mostafa.file"_cs, IO_MODE2::WRITE, OPEN_MODE::CREATE_OVERWRITE);
	auto a = my_file.size();
	auto b = my_file.cursor();
	auto r = platform.write_file(my_file.handle, str._data.view_bytes(0, str.size() - 1));
	io_trait* toto = my_file;
	r = toto->write(str._data.view_bytes(0, str.size() - 1));
	a = my_file.size();
	b = my_file.cursor();
	auto c = my_file.move(-2);
	b = my_file.cursor();
	c = my_file.move_to_start();
	b = my_file.cursor();
	c = my_file.move_to_end();
	b = my_file.cursor();
}

void read()
{
	file my_file = file::open("mostafa.file"_cs, IO_MODE2::READ, OPEN_MODE::OPEN_ONLY);
	
	auto buffer = platform.alloc<byte>(1024);
	auto r = platform.read_file(my_file.handle, buffer);

	println(std::cout, string(buffer.view(0, r), nullptr));
}

int
main(int argc, char** argv)
{
	write();
	read();
	return 0;
	do_benchmark();
	return 0;
}
