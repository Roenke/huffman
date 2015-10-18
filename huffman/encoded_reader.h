#pragma once
#include <vector>
#include "reader.h"

class encoded_reader : public reader
{
public:
    explicit encoded_reader(std::string const& cs);
    explicit encoded_reader(const encoded_reader& reader) = delete;
    encoded_reader& operator=(const encoded_reader&) = delete;
    ~encoded_reader();

    virtual void read_frequencies(std::vector<std::pair<uint8_t, size_t>>&) const override;
    virtual bool read_content(char*, size_t, size_t&) override;
    size_t get_system_info_size() const;
private:
    size_t eval_header_size(size_t) const;
};
