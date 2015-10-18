#include "parse_exception.h"

parse_exception::parse_exception(exception_type type)
{
	type_ = type;
}

parse_exception::parse_exception(parse_exception& parse_exception)
{
	type_ = parse_exception.type_;
}

parse_exception::~parse_exception()
{
}

std::string parse_exception::get_description() const
{
	switch(type_)
	{
	case wrong_number_of_arguments: return "Wrong number of parameters.";
	case wrong_mode_key: return "Wrong mode.";
	case wrong_input_file_key: return "Wrong input file key.";
	case wrong_output_file_key: return "Wrong output file key.";
	case in_file_not_exists: return "Input file not found. Check input file path.";
	}

	return "Unknown reason";
}
