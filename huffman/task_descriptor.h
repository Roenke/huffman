#pragma once
#include <string>

enum mode
{
    encode_mode,
    decode_mode
};

class task_descriptor
{
public:
    static const size_t in_buffers_size = 10 * 4096;
    static const size_t out_buffers_size = 10 * 4096;
    static const size_t alphabet_size = 256;
    task_descriptor(mode, std::string const& in, std::string const& out);
    task_descriptor(task_descriptor const&);
    task_descriptor& operator=(task_descriptor const&) = delete;
    ~task_descriptor();

    mode get_mode() const;
    std::string get_input_filename() const;
    std::string get_output_filename() const;
private:
    mode task_mode_;
    std::string input_filename_;
    std::string output_filename_;
};
