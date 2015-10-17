#pragma once
#include <string>
#include <fstream>
#include <vector>

class encoded_writer
{
public:
	explicit encoded_writer(std::string& filename);
	void encode_mapping(std::vector<std::pair<uint8_t, size_t>> const& mapping);
	void write_code(std::vector<bool> const& code);
	void flush();
private:
	static const uint8_t bits_per_byte;

	std::ofstream file;
	std::vector<bool> buffer;
};
