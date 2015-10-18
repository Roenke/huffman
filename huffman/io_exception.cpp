#include "io_exception.h"

io_exception::io_exception(std::string const& msg)
    : exception()
{
    message_ = msg;
}

std::string io_exception::get_description() const
{
    return message_;
}