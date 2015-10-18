#pragma once
#include <string>
#include <vector>
#include "writer.h"

class encoded_writer : public writer
{
public:
    explicit encoded_writer(const std::string& filename);
    explicit encoded_writer(const encoded_writer& reader) = delete;
    encoded_writer& operator=(const encoded_writer&) = delete;
    ~encoded_writer() override;

    void append_data(char*, size_t) override;
    void encode_char_mapping(std::vector<std::pair<uint8_t, size_t>> const&, std::streampos&) const;
};