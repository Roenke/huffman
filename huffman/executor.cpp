#include "executor.h"
#include "raw_reader.h"
#include "huffman_tree.h"
#include "encoded_writer.h"
#include "encoded_reader.h"
#include "raw_writer.h"
#include <iostream>

executor::executor(task_descriptor& task_descriptor)
    :task_descriptor_(task_descriptor)
{
}

uint8_t executor::reverse_bytes(uint8_t b) const
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

void executor::encode_file(std::vector<std::vector<bool>>& codes, raw_reader& reader, encoded_writer& writer) const
{
    auto input_buffer = new uint8_t[task_descriptor::in_buffers_size];
    auto output_buffer = new uint8_t[task_descriptor::out_buffers_size];
    size_t output_buffer_ptr = 0;
    size_t readed;
    uint8_t buf = 0;
    uint8_t bit_counter = 0;
    while (reader.read_content(reinterpret_cast<char*>(input_buffer), task_descriptor::in_buffers_size, readed))
    {
        for (size_t i = 0; i < readed; ++i)
        {
            for (auto& code : codes[input_buffer[i]])
            {
                if (bit_counter >= CHAR_BIT * sizeof(buf))
                {
                    output_buffer[output_buffer_ptr++] = reverse_bytes(buf);
                    bit_counter = 0;
                    buf = 0;
                    if (output_buffer_ptr >= task_descriptor::out_buffers_size)
                    {
                        writer.append_data(reinterpret_cast<char*>(output_buffer), output_buffer_ptr);
                        output_buffer_ptr = 0;
                    }
                }

                buf = buf << 1 | static_cast<uint8_t>(code);
                bit_counter++;
            }
        }
    }

    output_buffer[output_buffer_ptr++] = reverse_bytes(buf);

    writer.append_data(reinterpret_cast<char*>(output_buffer), output_buffer_ptr);

    delete[] input_buffer;
    delete[] output_buffer;
}

void executor::start() const
{
    if(task_descriptor_.get_mode() == decode_mode)
    {
        decode();
    }
    else
    {
        encode();
    }
}

void executor::decode() const
{
    using namespace std;
    auto input_filename = task_descriptor_.get_input_filename();
    encoded_reader reader(input_filename);

    vector<pair<uint8_t, size_t>> frequencies;

    reader.read_frequencies(frequencies);
    huffman_tree tree(frequencies);
    auto codes = tree.get_codes_mapping();

    auto output_filename = task_descriptor_.get_output_filename();
    raw_writer writer(output_filename);
    tree.decode(reader, writer);

    cout << reader.get_file_size() << endl;
    cout << writer.get_file_size() << endl;
    cout << reader.get_system_info_size() << endl;
}

void executor::encode() const
{
    using namespace std;
    raw_reader reader(task_descriptor_.get_input_filename());
    vector<pair<uint8_t, size_t>> frequencies;

    reader.read_frequencies(frequencies);

    huffman_tree tree(frequencies);

    auto output_filename = task_descriptor_.get_output_filename();
    encoded_writer writer(output_filename);
    auto codes = tree.get_codes_mapping();

    std::streampos sizeof_headers;
    writer.encode_char_mapping(frequencies, sizeof_headers);

    encode_file(codes, reader, writer);

    cout << reader.get_file_size() << endl;
    cout << writer.get_file_size() << endl;
    cout << sizeof_headers << endl;
}
