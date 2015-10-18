#include <iostream>
#include <vector>
#include "parameters_parser.h"
#include "parse_exception.h"
#include "raw_reader.h"
#include "io_exception.h"
#include "huffman_tree.h"
#include "encoded_writer.h"
#include "encoded_reader.h"
#include "raw_writer.h"
#include "task_descriptor.h"

void ShowUsage()
{
	std::cout << "Usage:" << std::endl;
	std::cout << "huffman [mode] [input-file-key] path/to/in_file.txt [output-file-key] path/to/new_out_file.txt" << std::endl;
	std::cout << "where" << std::endl << "\t[mode]:" << std::endl << "\t\t-c encode mode" << std::endl << "\t\t-u decode mode" << std::endl;
	std::cout << "\t[input-file-key]:" << std::endl << "\t\t-f, --file input file for encoding/decoding" << std::endl;
	std::cout << "\t[output-file-key]:" << std::endl << "\t\t-o, --output output file for encoding/decoding" << std::endl;
	std::cout << "Limits: Size of input file must be lesser then 5mb" << std::endl;
}

unsigned char reverse_bytes(unsigned char b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

void encode_file(std::vector<std::vector<bool>>& codes, raw_reader& reader, encoded_writer& writer)
{
	auto input_buffer = new uint8_t[4096];
	auto output_buffer = new uint8_t[4096];
	size_t output_buffer_ptr = 0;
	size_t readed;
	uint8_t buf = 0;
	uint8_t bit_counter = 0;
	while(reader.read_content(reinterpret_cast<char*>(input_buffer), 4096, readed))
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
					if (output_buffer_ptr >= 4096)
					{
						writer.append_data(reinterpret_cast<char*>(output_buffer), output_buffer_ptr);
						output_buffer_ptr = 0;
					}
				}

				buf = buf << 1 | code;
				bit_counter++;
			}
		}
	}

	output_buffer[output_buffer_ptr++] = reverse_bytes(buf);

	writer.append_data(reinterpret_cast<char*>(output_buffer), output_buffer_ptr);

	delete[] input_buffer;
	delete[] output_buffer;
}

void encode(task_descriptor descriptor)
{
	using namespace std;
	raw_reader reader(descriptor.get_input_filename());
	vector<pair<uint8_t, size_t>> frequencies;
	
	reader.read_frequencies(frequencies);

	huffman_tree tree(frequencies);

	auto output_filename = descriptor.get_output_filename();
	encoded_writer writer(output_filename);
	auto codes = tree.get_codes_mapping();
	writer.encode_char_mapping(frequencies);

	encode_file(codes, reader, writer);
}

void decode(task_descriptor descriptor)
{
	using namespace std;
	auto input_filename = descriptor.get_input_filename();
	encoded_reader reader(input_filename);

	vector<pair<uint8_t, size_t>> frequencies;

	reader.read_frequencies(frequencies);
	huffman_tree tree(frequencies);
	auto codes = tree.get_codes_mapping();

	auto output_filename = descriptor.get_output_filename();
	raw_writer writer(output_filename);
	tree.decode(reader, writer);
}

int main(int argc, char* argv[])
{
	using namespace std;
	vector<string> args;

	args.assign(argv + 1, argv + argc);
	auto parser = new parameters_parser();
	try
	{
		auto descriptor = parser->parse(args);

		if(descriptor.get_mode() == Decode)
		{
			decode(descriptor);
		}
		else
		{
			encode(descriptor);
		}
	}
	catch(parse_exception const& ex)
	{
		cerr << ex.get_description() << endl;
		ShowUsage();
		return 1;
	}
	catch(io_exception const& ex)
	{
		cerr << ex.get_description() << endl;
		ShowUsage();
		return 1;
	}
	
	return 0;
}
