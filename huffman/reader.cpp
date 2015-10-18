#include "reader.h"

reader::reader(std::string const& filename)
{
	filename_ = filename;
}

reader::~reader()
{
	if(opened_file_.is_open())
	{
		opened_file_.close();
	}
}
