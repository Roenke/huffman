#pragma once
#include <fstream>
#include "writer.h"

class raw_writer : public writer
{
public:
	explicit raw_writer(const std::string& filename);
	virtual void append_data(char*, size_t) override;

	virtual ~raw_writer() override;
};