#include "raw_reader.h"
#include "io_exception.h"
#include <fstream>
#include "task_descriptor.h"

raw_reader::raw_reader(std::string const& filename)
    :reader(filename)
{
}

raw_reader::~raw_reader()
{
}

void raw_reader::read_frequencies(std::vector<std::pair<uint8_t, size_t>>& frequencies) const
{
    std::ifstream file(filename_, std::fstream::binary);

    frequencies.resize(task_descriptor::alphabet_size);
    for (size_t i = 0; i < task_descriptor::alphabet_size; ++i)
    {
        frequencies[i].first = static_cast<uint8_t>(i);
        frequencies[i].second = 0;
    }

    auto buffer = new uint8_t[task_descriptor::in_buffers_size];
    while(file.read(reinterpret_cast<char*>(buffer), task_descriptor::in_buffers_size))
    {
        for (size_t i = 0; i < task_descriptor::in_buffers_size; ++i)
        {
            frequencies[buffer[i]].second++;
        }
    }

    auto yet_size = static_cast<size_t>(file.gcount());
    for (size_t i = 0; i < yet_size;  ++i)
    {
        frequencies[buffer[i]].second++;
    }
}

bool raw_reader::read_content(char* buffer, size_t buffer_size, size_t& readed)
{
    if (!opened_file_.is_open())
    {
        opened_file_.open(filename_, std::ifstream::binary);
        if (!opened_file_.good())
        {
            io_exception ex("Cannot open file" + filename_ + " for reading raw data.");
            throw ex;
        }
    }

    if (opened_file_.read(buffer, buffer_size) || opened_file_.gcount() > 0)
    {
        readed = static_cast<size_t>(opened_file_.gcount());
        return true;
    }

    return false;
}
