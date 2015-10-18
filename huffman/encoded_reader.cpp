#include <fstream>
#include "encoded_reader.h"
#include "io_exception.h"

encoded_reader::encoded_reader(std::string const& filename)
	:reader(filename)
{
}

void encoded_reader::read_frequencies(std::vector<std::pair<uint8_t, size_t>>& frequencies) const
{
	std::ifstream file(filename_, std::fstream::binary);
	uint16_t alphabet_size;
	file.read(reinterpret_cast<char*>(&alphabet_size), sizeof(alphabet_size));
	auto header_size = eval_header_size(alphabet_size);

	auto buffer = new char[header_size];
	file.read(buffer, header_size - sizeof(alphabet_size));

	frequencies.resize(0);
	uint8_t* character_ptr;
	size_t* long_fraquency_ptr;
	size_t offset;
	auto sizeof_symbol_section = sizeof(uint8_t) + sizeof(size_t);
	for (size_t i = 0; i < alphabet_size; ++i)
	{
		offset = i * sizeof_symbol_section;
		character_ptr = reinterpret_cast<uint8_t*>(&buffer[offset]);
		long_fraquency_ptr = reinterpret_cast<size_t*>(&buffer[offset + sizeof(uint8_t)]);
		frequencies.push_back(std::pair<uint8_t, size_t>(*character_ptr, *long_fraquency_ptr));
	}
}

bool encoded_reader::read_content(char* buffer, size_t buffer_size, size_t& readed)
{
	if(!opened_file_.is_open())
	{
		opened_file_.open(filename_, std::ifstream::binary);
		if(!opened_file_.good())
		{
			throw io_exception("Cannot to open file");
		}

		uint16_t alphabet_size;
		opened_file_.read(reinterpret_cast<char*>(&alphabet_size), sizeof(uint16_t));
		auto offset = eval_header_size(alphabet_size);
		opened_file_.seekg(offset, opened_file_.beg);
	}

	if(opened_file_.read(buffer, buffer_size) || opened_file_.gcount() > 0)
	{
		readed = opened_file_.gcount();
		return true;
	}

	return false;
}

encoded_reader::~encoded_reader()
{
	opened_file_.close();
}

size_t encoded_reader::eval_header_size(size_t alphabet_size) const
{
	return sizeof(uint16_t) + alphabet_size * (sizeof(size_t) + sizeof(uint8_t));
}
