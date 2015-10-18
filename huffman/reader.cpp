#include "reader.h"
#include "io_exception.h"

std::streampos reader::get_file_size() const
{
    std::ifstream file(filename_, std::ifstream::ate | std::ifstream::binary);
    if(!file.good())
    {
        throw io_exception("Cannot open file");
    }

    return file.tellg();
}

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
