#include "benchmark.h"
#include <cpprelude/stream.h>
#include <cpprelude/string.h>
#include <sstream>
using namespace cpprelude;

void trait(stream_trait* stream)
{
	usize result = write(stream, 324);
	result = write_str(stream, -567);

	int x;
	result = read(stream, x);

	string xoxo = "my name is mostafa"_cs;
	result = write(stream, xoxo);

	string buffer(KILOBYTES(1));
	result = read(stream, buffer);
}

void file_stuff(stream_trait* stream)
{
	usize result = write_str(stream, 123);
	result = write_str(stream, -567);

	string xoxo = "my name is mostafa"_cs;
	result = write_str(stream, xoxo);

	string buffer(KILOBYTES(1));
	result = read(stream, buffer);
}

void space()
{
	memory_stream buffer;
	trait(buffer);
	memory_stream x = std::move(buffer);

	file_stream file;
	auto txt_file = open_file("data.txt"_cs, IO_MODE::APPEND_EXTENDED);
	auto result = txt_file.size();
	file_stuff(txt_file);
	result = txt_file.size();
	return;
}

int
main(int argc, char** argv)
{
	space();
	do_benchmark();
	return 0;
}
