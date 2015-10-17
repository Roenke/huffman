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

void encode_file(std::vector<std::vector<bool>>& codes, raw_reader& reader, encoded_writer& writer)
{
	auto content = reader.get_content();
	
	auto length = content.size();
	for (size_t i = 0; i < length; ++i)
	{
		writer.write_code(codes[content[i]]);
	}

	writer.flush();
}

void encode(task_descriptor descriptor)
{
	using namespace std;
	raw_reader reader(descriptor.get_input_filename());
	vector<pair<uint8_t, size_t>> frequencies(256);
	for (size_t i = 0; i < 256; ++i)
	{
		frequencies[i].first = static_cast<uint8_t>(i);
		frequencies[i].second = 0;
	}
	
	reader.read_frequencies(frequencies);

	for (size_t i = 0; i < 256; ++i)
	{
		if (frequencies[i].second != 0)
		{
			cerr << i << " : " << frequencies[i].second << endl;
		}
	}

	huffman_tree tree(frequencies);

	auto output_filename = descriptor.get_output_filename();
	encoded_writer writer(output_filename);
	auto codes = tree.get_codes_mapping();
	writer.encode_mapping(frequencies);

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
	vector<bool> binary_content;
	reader.get_content_as_bits(binary_content);
	vector<uint8_t> content;
	tree.decode(binary_content, content);

	auto output_filename = descriptor.get_output_filename();
	raw_writer writer(output_filename);
	writer.write_bytes(content);
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
