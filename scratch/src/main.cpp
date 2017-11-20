#include "benchmark.h"
#include <cpprelude/stream.h>
#include <cpprelude/string.h>
#include <sstream>
using namespace cpprelude;

struct v2
{
	float x, y;
};

inline static usize
write_str(stream_trait* stream, const v2& value)
{
	return vwrite_str(stream, "v2{ "_cs, value.x, ", ", value.y, " };");
}

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
	usize result = vwrite_str(stream, 123);
	result = write_str(stream, -567);

	string xoxo = "my name is mostafa"_cs;
	result = write_str(stream, xoxo);

	string buffer(KILOBYTES(1));
	result = read(stream, buffer);

	v2 vv{ 1, 23 };
	result = write_str(stream, vv);
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
