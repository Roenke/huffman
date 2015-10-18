#include "parameters_parser.h"
#include <vector>
#include <fstream>
#include "parse_exception.h"

const std::string parameters_parser::encode_key_ = "-c";
const std::string parameters_parser::decode_key_ = "-u";
const std::string parameters_parser::in_file_key_ = "-f";
const std::string parameters_parser::in_file_long_key_ = "--file";
const std::string parameters_parser::out_file_key_ = "-o";
const std::string parameters_parser::out_file_long_key_ = "--output";

parameters_parser::parameters_parser()
{
}

task_descriptor parameters_parser::parse(std::vector<std::string> const& argv)
{
    auto argc = argv.size();
    if(argc != 5)
    {
        parse_exception ex(static_cast<exception_type>(wrong_number_of_arguments));
        throw ex;
    }

    std::string in_filename;
    try_in_file_correct(argv, in_filename);

    std::string out_filename;
    try_out_file_correct(argv, out_filename);

    mode mode;
    try_find_mode(argv, mode);

    return task_descriptor(mode, in_filename, out_filename);
}

bool parameters_parser::is_file_exists(std::string const& filename)
{
    std::ifstream file(filename);
    auto result = file.good();
    file.close();
    return result;
}

void parameters_parser::try_in_file_correct(std::vector<std::string> const& argv, std::string& filename)
{
    auto size_args = argv.size();
    for (size_t i = 0; i < size_args - 1; ++i)
    {
        if (argv[i] == in_file_key_ || argv[i] == in_file_long_key_)
        {
            if(!is_file_exists(argv[i + 1]))
            {
                parse_exception ex(in_file_not_exists);
                throw ex;
            }

            filename = argv[i + 1];
            return;
        }
    }

    parse_exception ex(wrong_input_file_key);
    throw ex;
}

void parameters_parser::try_out_file_correct(std::vector<std::string> const& argv, std::string& filename)
{
    auto size_args = argv.size();
    for (size_t i = 0; i < size_args - 1; ++i)
    {
        if(argv[i] == out_file_key_ || argv[i] == out_file_long_key_)
        {
            filename = argv[i + 1];
            return;
        }
    }

    parse_exception ex(wrong_output_file_key);
    throw ex;
}

void parameters_parser::try_find_mode(std::vector<std::string> const& argv, mode &mode)
{
    auto size_args = argv.size();
    for (size_t i = 0; i < size_args; ++i)
    {
        if(argv[i] == encode_key_)
        {
            mode = encode_mode;
            return;
        }
        
        if(argv[i] == decode_key_)
        {
            mode = decode_mode;
            return;
        }
    }

    parse_exception ex(wrong_mode_key);
    throw ex;
}
