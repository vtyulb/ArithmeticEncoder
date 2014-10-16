//AR is ARithmetic, not an ARchive

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "ar_encoder.h"
#include "ar_decoder.h"

namespace po=boost::program_options;

void showHelp() {
    printf("Arithmetic encoder. Usage:\n");
    printf("  ./ArithmeticEncoder <-c [-p] | -u> [-i input file] [-o output file] \n\n");
    printf("\t-c --compress         compress target\n");
    printf("\t-u --unpack           unpack target\n");
    printf("\t-i --input <string>   Input (defaults to stdin)\n");
    printf("\t-o --output <string>  Output (defaults to stdout)\n");
    printf("\t-p --ppm              Use PPM compression\n");
    printf("\t-h --help\n");
    printf("\nQuestions, suggestions, bugs please report to <vtyulb@vtyulb.ru>\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    std::string output;
    std::string input;

    po::options_description desc("General options");
    desc.add_options()
            ("compress,c", "Compress target")
            ("unpack,u", "Unpack target")
            ("input,i", po::value<std::string>(&input), "Input (defaults to stdin)")
            ("output,o", po::value<std::string>(&output), "Output (defaults to stdout)")
            ("ppm,p", "Use powerfull PPM compression")
            ("help,h", "Show help");

    po::variables_map vm;
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    po::store(parsed, vm);
    po::notify(vm);

    if (vm.count("help"))
        showHelp();


    FILE *out;
    if (vm.count("output")) {
        out = fopen(output.data(), "w");
        assert(out);
    } else
        out = stdout;

    FILE *in;
    if (vm.count("input")) {
        in = fopen(input.data(), "r");
        assert(in);
    } else
        in = stdin;

    if (vm.count("compress")) {
        AR_Encoder e(vm.count("ppm"));
        int ch;
        while ((ch = getc(in)) != EOF)
            e.putSymbol((char)ch);

        std::vector<char> res = e.getEncodedResult();

        fwrite(res.data(), sizeof(AR_symbol), res.size(), out);
        fclose(out);

        return 0;
    }

    if (vm.count("unpack")) {
        std::vector<char> data;
        int ch;
        while ((ch = getc(in)) != EOF)
            data.push_back(ch);

        AR_Decoder d(data);
        std::vector<AR_symbol> decoded = d.getDecoded();

        fwrite(decoded.data(), sizeof(AR_symbol), decoded.size(), out);
        fclose(out);

        return 0;
    }

    printf("Target did't recognized. Displaying standart help.\n");
    showHelp();

    return 0;
}
