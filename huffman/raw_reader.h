#pragma once
#include <string>
#include <vector>

class raw_reader
{
public:
	explicit raw_reader(std::string const& filename);
	void read_frequencies(std::vector<std::pair<uint8_t, size_t>>& frequencies);
	std::vector<uint8_t> const& raw_reader::get_content();

	~raw_reader();
private:
	static const size_t file_size_limit;
	std::string filename_;
	std::vector<uint8_t> buffer_;

	size_t read_all_into_buffer();
};
