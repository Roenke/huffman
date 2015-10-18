#pragma once
#include <string>
#include <vector>
#include "reader.h"

class raw_reader : public reader
{
public:
    explicit raw_reader  (std::string const& cs);
    explicit raw_reader  (const reader& reader) = delete;
    raw_reader& operator=(const raw_reader&)    = delete;
    virtual ~raw_reader  ();

    virtual void read_frequencies(std::vector<std::pair<uint8_t, size_t>>&) const override;
    virtual bool read_content    (char*, size_t, size_t&) override;
};