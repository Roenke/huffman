#pragma once
#include <exception>
#include <string>

enum exception_type
{
	wrong_number_of_arguments,
	wrong_mode_key,
	wrong_input_file_key,
	wrong_output_file_key,
	in_file_not_exists,
	out_file_exists
};

class parse_exception : std::exception
{
public:
	explicit parse_exception(exception_type);
	std::string get_description() const;
private:
	exception_type type_;
};
