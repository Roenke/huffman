#include "raw_reader.h"
#include "io_exception.h"
#include <fstream>

const size_t raw_reader::file_size_limit = 10 * 1024 * 1024; // 5 megabytes

raw_reader::raw_reader(std::string const& filename)
{
	filename_ = filename;
}

void raw_reader::read_frequencies(std::vector<std::pair<uint8_t, size_t>>& frequencies)
{
	read_all_into_buffer();

	auto length = buffer_.size();
	for (size_t i = 0; i < length; ++i)
	{
		frequencies[buffer_[i]].second++;
	}
}

std::vector<uint8_t> const& raw_reader::get_content()
{
	if(buffer_.size() == 0)
	{
		read_all_into_buffer();
	}
	
	return buffer_;
}

raw_reader::~raw_reader()
{
}


size_t raw_reader::read_all_into_buffer()
{
	std::ifstream file;
	file.open(filename_, std::ifstream::binary);

	if (!file.good())
	{
		throw io_exception("Can't open input file");
	}

	file.seekg(0, file.end);
	size_t file_length = file.tellg();
	file.seekg(0, file.beg);

	if (file_length > file_size_limit)
	{
		throw io_exception("File very large. Limit 5 mb.");
	}

	buffer_.resize(file_length);
	file.read(reinterpret_cast<char*>(&buffer_[0]), file_length);
	return file_length;
}