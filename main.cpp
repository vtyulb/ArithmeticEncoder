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

void compress(FILE *in, FILE *out, int ppm) {
    std::vector<AR_symbol> data;
    int ch;
    while ((ch = getc(in)) != EOF)
        data.push_back((unsigned char)(ch));


    const int blck = 512;
    const int diff = 50;

    int *prev = new  int[256];
    int *current = new int[256];
    double *rt = new double[data.size() / blck];
    rt[0] = 0;

    for (unsigned i = 1; i < data.size() / blck - 1; i++) {
        for (int j = 0; j < 256; j++) {
            prev[j] = 0;
            current[j] = 0;
        }

        for (unsigned j = (i - 1) * blck; j < i * blck; j++)
            prev[data[j]]++;

        for (unsigned j = (i * blck); j < (i + 1) * blck; j++)
            current[data[j]]++;

        double res = 0;
        for (int j = 0; j < 256; j++)
            res += (prev[j] - current[j]) * (prev[j] - current[j]) / 1000.0;

        rt[i] = res;
    }

//    for (int i = 1; i < data.size() / blck - 1; i++)
//        if (rt[i] > diff)
//            fprintf(stderr, "blck %d: %.3g\n", i, rt[i]);

    for (unsigned i = 0; i < data.size(); i++)
        current[data[i]]++;

    bool txt = false;
    if (current[32] > data.size() / 40) {
        txt = true;
//        fprintf(stderr, "txt file!");
    }

    AR_Encoder e(ppm, txt);

    for (unsigned i = 0; i < data.size(); i++) {
        if (rt[i / blck] > diff && i % blck == 0)
            e.putSymbol(256);

        e.putSymbol(data[i]);
    }
    std::vector<char> res(e.getEncodedResult());

    fwrite(res.data(), 1, res.size(), out);
    fclose(out);
//    exit(0);
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
//    exit(0);
}

int main(int argc, char *argv[]) {
    FILE *fin = fopen(argv[2], "r");
    FILE *fout = fopen(argv[3], "w");
    int ppm = (strcmp(argv[4], "ppm") == 0);

    if (argv[1][0] == 'c')
        compress(fin, fout, ppm);
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
