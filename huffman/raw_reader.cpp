#include "raw_reader.h"
#include "io_exception.h"
#include <fstream>

raw_reader::raw_reader(std::string const& filename)
	:reader(filename)
{
}

raw_reader::~raw_reader()
{
}

void raw_reader::read_frequencies(std::vector<std::pair<uint8_t, size_t>>& frequencies) const
{
	std::ifstream file(filename_, std::fstream::binary);

	frequencies.resize(256);
	for (size_t i = 0; i < 256; ++i)
	{
		frequencies[i].first = static_cast<uint8_t>(i);
		frequencies[i].second = 0;
	}

	auto buffer = new uint8_t[4096];
	while(file.read(reinterpret_cast<char*>(buffer), 4096))
	{
		for (size_t i = 0; i < 4096; ++i)
		{
			frequencies[buffer[i]].second++;
		}
	}

	for (size_t i = 0; i < file.gcount();  ++i)
	{
		frequencies[buffer[i]].second++;
	}
}

bool raw_reader::read_content(char* buffer, size_t buffer_size, size_t& readed)
{
	if (!opened_file_.is_open())
	{
		opened_file_.open(filename_, std::ifstream::binary);
		if (!opened_file_.good())
		{
			throw io_exception("Cannot to open file");
		}
	}

	if (opened_file_.read(buffer, buffer_size) || opened_file_.gcount() > 0)
	{
		readed = static_cast<size_t>(opened_file_.gcount());
		return true;
	}

	return false;
}
