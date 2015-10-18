#include "raw_writer.h"
#include <iostream>

raw_writer::raw_writer(std::string const& filename)
{
	file.open(filename, std::ofstream::binary | std::ofstream::trunc);
}

void raw_writer::write_bytes(std::vector<uint8_t>& content)
{
	auto content_size = content.size();
	file.seekp(file.end);
	std::cerr << "File size before writing = " << file.tellp() << std::endl;
	std::cerr << "Content size = " << content_size << std::endl;
	file.seekp(file.beg);
	for (size_t i = 0; i < content_size; ++i)
	{
		file.write(reinterpret_cast<char*>(&content[i]), 1);
	}
}
