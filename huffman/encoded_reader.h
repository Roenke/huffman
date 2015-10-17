#pragma once
#include <vector>
#include "reader.h"

class encoded_reader
{
public:
	explicit encoded_reader(std::string const& filename);
	void read_frequencies(std::vector<std::pair<uint8_t, size_t>> &);
	void get_content_as_bits(std::vector<bool> &);
private:
	std::ifstream file;
};

class encoded_reader1 : public reader
{
public:
	explicit encoded_reader1(std::string const& cs);
	explicit encoded_reader1(const reader& reader);
	void read_frequencies(std::vector<std::pair<uint8_t, size_t>>&) const override;
	bool read_content(char*, size_t) override;
protected:
	~encoded_reader1();
private:
	size_t eval_header_size(size_t) const;
};