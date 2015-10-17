#include <fstream>
#include "encoded_reader.h"
#include "io_exception.h"

encoded_reader::encoded_reader(std::string const& filename)
{
	file.open(filename, std::fstream::in | std::fstream::binary);
}

void encoded_reader::read_frequencies(std::vector<std::pair<uint8_t, size_t>>& frequencies)
{
	uint16_t alphabet_size;
	file.read(reinterpret_cast<char*>(&alphabet_size), sizeof(uint16_t));

	frequencies.resize(0);
	char character;
	auto frequency = new char[4];

	size_t* long_fraquency;
	for (size_t i = 0; i < alphabet_size; ++i)
	{
		file.read(&character, 1);
		file.read(frequency, 4);
		long_fraquency = reinterpret_cast<size_t*>(frequency);
		frequencies.push_back(std::pair<uint8_t, size_t>(character, *long_fraquency));
	}
}

void encoded_reader::get_content_as_bits(std::vector<bool>& binary_content)
{
	uint16_t alphabet_size;
	file.seekg(0, file.beg);
	file.read(reinterpret_cast<char*>(&alphabet_size), sizeof(uint16_t));
	
	auto offset = 2 + alphabet_size * (sizeof(uint8_t) + sizeof(size_t));
	file.seekg(offset, file.beg);

	//std::vector<uint8_t> buffer(content_size);

	//file.read(&buffer[0], content_size);

	uint8_t value;
	while (file.read(reinterpret_cast<char*>(&value), 1))
	{
		for (size_t i = 0; i < CHAR_BIT; i++)
		{
			binary_content.push_back(((value >> i) & 1) != 0);
		}
	}
}

void encoded_reader1::read_frequencies(std::vector<std::pair<uint8_t, size_t>>& frequencies) const
{
	std::ifstream file(filename_, std::fstream::binary);
	uint16_t alphabet_size;
	file.read(reinterpret_cast<char*>(&alphabet_size), sizeof(uint16_t));
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

bool encoded_reader1::read_content(char* buffer, size_t buffer_size)
{
	if(!openned_file_.is_open())
	{
		openned_file_.open(filename_);
		if(!openned_file_.good())
		{
			throw io_exception("Cannot to open file");
		}

		uint16_t alphabet_size;
		openned_file_.read(reinterpret_cast<char*>(&alphabet_size), sizeof(uint16_t));
		auto offset = eval_header_size(alphabet_size);
		openned_file_.seekg(offset, openned_file_.beg);
	}

	if(openned_file_.read(buffer, buffer_size))
	{
		return true;
	}
	
	return false;
}

encoded_reader1::~encoded_reader1()
{
	openned_file_.close();
}

size_t encoded_reader1::eval_header_size(size_t alphabet_size) const
{
	return sizeof(uint16_t) + alphabet_size * (sizeof(size_t) + sizeof(uint8_t));
}
