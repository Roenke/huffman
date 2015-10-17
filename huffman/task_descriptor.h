#pragma once
#include <string>

enum mode
{
	Encode,
	Decode
};

class task_descriptor
{
public:
	task_descriptor(mode, std::string const& in, std::string const& out);
	mode get_mode() const;
	std::string get_input_filename() const;
	std::string get_output_filename() const;
private:
	mode task_mode_;
	std::string input_filename_;
	std::string output_filename_;
};
