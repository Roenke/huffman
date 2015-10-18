#pragma once
#include <string>
#include <fstream>

class writer
{
public:
	explicit writer(std::string filename);
	virtual void append_data(char*, size_t) = 0;
	virtual ~writer();
protected:
	std::string filename_;
	std::ofstream opened_file_;
};
