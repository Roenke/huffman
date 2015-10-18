#pragma once
#include <fstream>
#include "writer.h"

class raw_writer : public writer
{
public:
	explicit raw_writer(const std::string& filename);
	explicit raw_writer(writer &) = delete;
	writer& operator=(writer&) = delete;
	~raw_writer();

	virtual void append_data(char*, size_t) override;
};