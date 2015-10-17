#include "reader.h"
#include "io_exception.h"

reader::reader(std::string const& filename)
{
	filename_ = filename;
}

reader::~reader()
{
	if(openned_file_.is_open())
	{
		openned_file_.close();
	}
}
