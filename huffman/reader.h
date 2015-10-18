#pragma once
#include <string>
#include <vector>
#include <fstream>

class reader
{
public:
	virtual void read_frequencies(std::vector<std::pair<uint8_t, size_t>> &) const = 0;
	virtual bool read_content(char*, size_t, size_t&) = 0;
protected:
	explicit reader(std::string const& );
	explicit reader(const reader&) = delete;
	reader& operator=(const reader&) = delete;
	virtual ~reader();
	std::string filename_;
	std::ifstream opened_file_;
};
