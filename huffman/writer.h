#pragma once
#include <string>
#include <fstream>

class writer
{
public:
	explicit writer(std::string filename);
	writer(writer const&) = delete;
	writer& operator=(writer const&) = delete;
	virtual ~writer();

	std::streampos get_file_size() const;
	virtual void append_data(char*, size_t) = 0;
	
protected:
	std::string filename_;
	std::ofstream opened_file_;
};
