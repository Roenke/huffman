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
};

class parse_exception 
    : public std::exception
{
public:
    explicit parse_exception    (exception_type);
    parse_exception             (parse_exception&) = default;
    parse_exception& operator = (parse_exception&) = delete;
    ~parse_exception            ()                 = default;

    std::string get_description() const;
private:
    exception_type type_;
};
