#include <iostream>
#include <vector>
#include "parameters_parser.h"
#include "parse_exception.h"
#include "io_exception.h"
#include "task_descriptor.h"
#include "executor.h"

void ShowUsage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "huffman [mode] [input-file-key] path/to/in_file.txt [output-file-key] path/to/new_out_file.txt" << std::endl;
    std::cout << "where" << std::endl << "\t[mode]:" << std::endl << "\t\t-c encode mode" << std::endl << "\t\t-u decode mode" << std::endl;
    std::cout << "\t[input-file-key]:" << std::endl << "\t\t-f, --file input file for encoding/decoding" << std::endl;
    std::cout << "\t[output-file-key]:" << std::endl << "\t\t-o, --output output file for encoding/decoding" << std::endl;
    std::cout << "Limits: Size of input file must be lesser then 5mb" << std::endl;
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

        executor exec(descriptor);
        exec.start();
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
