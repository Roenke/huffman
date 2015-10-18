#include "encoded_writer.h"
#include "io_exception.h"

encoded_writer::~encoded_writer()
{
}

void encoded_writer::append_data(char* data, size_t data_size)
{
	if(!opened_file_.is_open())
	{
		opened_file_.open(filename_, std::ofstream::app | std::fstream::binary);
		opened_file_.seekp(0, opened_file_.end);
		if(!opened_file_.good())
		{
			throw io_exception("Cannot open file");
		}
	}

	opened_file_.write(data, data_size);
}

void encoded_writer::encode_char_mapping(std::vector<std::pair<uint8_t, size_t>> const& char_mapping) const
{
	auto sizeof_mapping = char_mapping.size();
	uint16_t characters_count = 0;

	std::ofstream file(filename_, std::ofstream::binary | std::ofstream::trunc);
	file.seekp(sizeof(characters_count), file.beg);
	uint8_t character;
	size_t frequency;
	for (size_t i = 0; i < sizeof_mapping; ++i)
	{
		if (char_mapping[i].second != 0)
		{
			characters_count++;
			character = char_mapping[i].first;
			frequency = char_mapping[i].second;
			file.write(reinterpret_cast<char*>(&character), sizeof(character));
			file.write(reinterpret_cast<char*>(&frequency), sizeof(frequency));
		}
	}

	file.seekp(0, file.beg);
	file.write(reinterpret_cast<char*>(&characters_count), sizeof(characters_count));
	file.close();
}

encoded_writer::encoded_writer(const std::string& filename)
	: writer(filename)
{
}
