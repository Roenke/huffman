#pragma once
#include <exception>
#include <string>

class io_exception : public std::exception
{
public:
	explicit io_exception(std::string const& msg);
	std::string get_description() const;
private:
	std::string message;
};
