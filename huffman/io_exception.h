#pragma once
#include <exception>
#include <string>

class io_exception 
    : public std::exception
{
public:
    explicit io_exception    (std::string const&);
    io_exception             (io_exception&)  = default;
    io_exception& operator = (io_exception&)  = default;
    std::string get_description()               const;
private:
    std::string message_;
};
