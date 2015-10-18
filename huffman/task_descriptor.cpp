#include "task_descriptor.h"

task_descriptor::task_descriptor(mode programMode, std::string const& in, std::string const& out)
{
    task_mode_ = programMode;
    input_filename_ = in;
    output_filename_ = out;
}

task_descriptor::task_descriptor(task_descriptor const&task_descriptor)
{
    input_filename_ = task_descriptor.input_filename_;
    output_filename_ = task_descriptor.output_filename_;
    task_mode_ = task_descriptor.task_mode_;
}

task_descriptor::~task_descriptor()
{
}

mode task_descriptor::get_mode() const
{
    return task_mode_;
}

std::string task_descriptor::get_input_filename() const
{
    return input_filename_;
}

std::string task_descriptor::get_output_filename() const
{
    return output_filename_;
}