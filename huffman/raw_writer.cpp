#include "raw_writer.h"
#include "io_exception.h"

raw_writer::raw_writer(const std::string& filename)
	: writer(filename)
{
}

void raw_writer::append_data(char* data, size_t data_size)
{
	if(!opened_file_.is_open())
	{
		opened_file_.open(filename_, std::ofstream::app | std::ofstream::binary);
		if(!opened_file_.good())
		{
			throw io_exception("Cannot open file");
		}
	}

	opened_file_.write(data, data_size);
}

raw_writer::~raw_writer()
{
	opened_file_.close();
}