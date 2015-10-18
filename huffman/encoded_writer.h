#pragma once
#include <string>
#include <vector>
#include "writer.h"


class encoded_writer : public writer
{
public:
	explicit encoded_writer(const std::string& filename);
	void append_data(char*, size_t) override;
	void encode_char_mapping(std::vector<std::pair<uint8_t, size_t>> const&) const;
};