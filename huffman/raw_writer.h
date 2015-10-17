#pragma once
#include <fstream>
#include <vector>

class raw_writer
{
public:
	explicit raw_writer(std::string const &);
	void write_bytes(std::vector<uint8_t>& content);
private:
	std::ofstream file;
};
