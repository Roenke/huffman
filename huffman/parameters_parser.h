#pragma once
#include <vector>
#include "task_descriptor.h"

class parameters_parser
{
public:
    parameters_parser();
    parameters_parser(parameters_parser&) = delete;
    parameters_parser& operator=(parameters_parser&) = delete;
    ~parameters_parser() = delete;

    static task_descriptor parse(std::vector<std::string>const& argv);
private:
    static const std::string encode_key_;
    static const std::string decode_key_;
    static const std::string in_file_key_;
    static const std::string in_file_long_key_;
    static const std::string out_file_key_;
    static const std::string out_file_long_key_;

    static bool is_file_exists(std::string const& filename);

    static void try_in_file_correct(std::vector<std::string>const&, std::string& filename);
    static void try_out_file_correct(std::vector<std::string>const&, std::string& filename);
    static void try_find_mode(std::vector<std::string>const&, mode &);
};
