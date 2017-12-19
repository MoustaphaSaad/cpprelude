#include "benchmark.h"
#include <cpprelude/platform.h>
#include <cpprelude/string.h>
#include <cpprelude/file.h>
using namespace cpprelude;

void write()
{
	auto str = "KOKO"_cs;
	file my_file = file::open("mostafa.file"_cs, IO_MODE::WRITE, OPEN_MODE::CREATE_OVERWRITE);
	auto a = my_file.size();
	auto b = my_file.cursor();
	auto r = platform.file_write(my_file.handle, str._data.view_bytes(0, str.size() - 1));
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
	file my_file = file::open("mostafa.file"_cs, IO_MODE::READ, OPEN_MODE::OPEN_ONLY);
	
	auto buffer = platform.alloc<byte>(1024);
	auto r = platform.file_read(my_file.handle, buffer);

	println(string(buffer.view(0, r), nullptr));
}

struct v3
{
	i32 x, y, z;
};

inline static usize
print_str(io_trait *trait, const v3& value)
{
	return vprints(trait, value.x, value.y, value.z);
}

void
print2()
{
	file my_file = file::open("mostafa.file"_cs);
	io_trait* io = my_file;
	vprints(io, 1, 2, 3, v3{ 4, 5, 6 }, "abcdefg"_cs, "hijklmn");
}

void
read2()
{
	file my_file = file::open("mostafa.file"_cs, IO_MODE::READ, OPEN_MODE::OPEN_ONLY);
	io_trait* io = my_file;
	string buffer(KILOBYTES(1));
	vreads(io, buffer);
	println(buffer);

	string buffer2(4);
	auto sdf = vreads(cppr_in, buffer2);
	println(buffer2);
	sdf = vreads(cppr_in, buffer2);
	println(buffer2);
	//println(std::cerr, buffer);
}

int
main(int argc, char** argv)
{
	/*write();
	read();
	print2();
	read2();
	return 0;*/
	do_benchmark();
	return 0;
}
