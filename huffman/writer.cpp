#include "writer.h"

writer::writer(std::string filename)
{
	filename_ = filename;
}

writer::~writer()
{
	opened_file_.close();
}
