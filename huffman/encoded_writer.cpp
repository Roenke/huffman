#include "encoded_writer.h"

const uint8_t encoded_writer::bits_per_byte = 8;

encoded_writer::encoded_writer(std::string& filename)
{
	file.open(filename, std::ofstream::binary | std::ofstream::out);
}

void encoded_writer::encode_mapping(std::vector<std::pair<uint8_t, size_t>> const& frequencies)
{
	auto charset_size = frequencies.size();
	uint16_t char_count = 0;
	for (size_t i = 0; i < charset_size; ++i)
	{
		if(frequencies[i].second != 0)
		{
			char_count++;
		}
	}

	file.write(reinterpret_cast<char*>(&char_count), sizeof(uint16_t));
	for (size_t i = 0; i < charset_size; i++)
	{
		if(frequencies[i].second != 0)
		{
			file.write(reinterpret_cast<const char*>(&frequencies[i].first), sizeof(uint8_t));
			file.write(reinterpret_cast<const char*>(&frequencies[i].second), sizeof(size_t));
		}
	}
	buffer.resize(0);
}

void encoded_writer::write_code(std::vector<bool> const& code)
{
	auto size = code.size();
	for (size_t i = 0; i < size; ++i)
	{
		buffer.push_back(code[i]);
	}
}

// TODO внести внутрь класса
unsigned char reverse(unsigned char b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

void encoded_writer::flush()
{
	uint8_t buf = 0;
	uint8_t c = 0;
	auto buffer_size = buffer.size();

	for (size_t i = 0; i < buffer_size; ++i)
	{
		if (c >= bits_per_byte * sizeof(buf))
		{
			buf = reverse(buf);
			file.write(reinterpret_cast<char*>(&buf), 1);
			c = 0;
			buf = 0;
		}

		buf = (buf << 1) | buffer[i];
		c++;
	}

	buf = reverse(buf);
	file.write(reinterpret_cast<char*>(&buf), 1);
	file.close();
}
