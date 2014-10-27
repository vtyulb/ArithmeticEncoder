//AR is ARithmetic, not an ARchive

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//#include <boost/program_options.hpp>
//#include <boost/algorithm/string.hpp>

#include "ar_encoder.h"
#include "ar_decoder.h"

#include "dllparser.h"
#include "bwt.h"

//namespace po=boost::program_options;

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

void compress(FILE *in, FILE *out, int ppm, int bwt) {
    std::vector<char> data;
    int ch;
    while ((ch = getc(in)) != EOF)
        data.push_back((unsigned char)ch);

    int spaces = 0;
    for (unsigned i = 0; i < data.size(); i++)
        spaces += (data[i] == 32);

    bool txt = false;
    if (spaces > data.size() / 40) {
        txt = true;
//        fprintf(stderr, "txt file!");
    }

    if (data.size() < 100)
        bwt = false;

    AR_Encoder e(ppm, txt, bwt);

    if (bwt)
        data = BWT_Direct(data);

    std::vector<int> sections = getSections(data);

    for (unsigned i = 0; i < data.size(); i++) {
        if (ppm)
            for (int j = 0; j < sections.size(); j++)
                if (sections[j] == i) {
                    e.putSymbol(256);
//                    fprintf(stderr, "flush %d\n", i);
                    break;
                }

        e.putSymbol((unsigned char)data[i]);
    }
    std::vector<char> res(e.getEncodedResult());

    fwrite(res.data(), 1, res.size(), out);
    fclose(out);
    exit(0);
}

void decompress(FILE *in, FILE *out) {
    std::vector<char> data;
    int ch;
    while ((ch = getc(in)) != EOF)
        data.push_back(ch);

    AR_Decoder d(data);
    std::vector<char> decoded(d.getDecoded());

    fwrite(decoded.data(), 1, decoded.size(), out);
    fclose(out);
    exit(0);
}

int main(int argc, char *argv[]) {
    FILE *fin = fopen(argv[2], "r");
    FILE *fout = fopen(argv[3], "w");

    if (argv[1][0] == 'c')
        compress(fin, fout, strstr(argv[4], "ppm") != NULL, strstr(argv[4], "bwt") != NULL);
    else
        decompress(fin, fout);


    /*
    std::string output;
    std::string input;

    po::options_description desc("General options");
    desc.add_options()
            ("compress,c", "Compress target")
            ("decompress,d", "Decompress target")
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
        compress(in, out, vm.count("ppm"));
        return 0;
    }

    if (vm.count("decompress")) {
        decompress(in, out);
        return 0;
    }

    printf("Target did't recognized. Displaying standart help.\n");
    showHelp();*/

    return 0;
}
