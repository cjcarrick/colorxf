#include "./lib/args.h"
#include "./lib/color.h"
#include "./lib/mix.h"
#include "./lib/transforms.h"
#include "./lib/util.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <regex>
#include <string>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;

int main(int argc, char **argv)
{
    auto transforms = vector<transforms::ColorTransform>();
    auto colors = vector<Color>();
    Args args(argv, argc);
    args.set_bool_opt("e");
    args.set_bool_opt("E");
    args.set_bool_opt("l");
    args.set_bool_opt("extract");
    args.set_bool_opt("extract-pretty");
    args.set_bool_opt("highlight");

    vector<string> mix_strs;
    args.get("m", mix_strs);
    args.get("mix", mix_strs);
    vector<mix::MixData> mixes;
    for (auto mix_str : mix_strs) mix::from_str(mix_str, mixes);

    vector<string> xf_strs;
    args.get("x", xf_strs);
    args.get("xf", xf_strs);
    vector<transforms::ColorTransform> xfs;
    for (auto xf_str : xf_strs) transforms::from_str(xf_str, xfs);

    if (args.get("help") || args.get('h')) {
        print_help();
        exit(0);
    }

    if (args.get("version") || args.get('v')) {
        cout << "0.4.0" << endl;
        exit(0);
    }

    enum {
        EXTRACT,
        EXTRACT_PRETTY,
        HIGHLIGHT
    } mode = HIGHLIGHT;

    if (args.get('E') || args.get("extract-pretty")) {
        mode = EXTRACT;
    } else if (args.get('e') || args.get("extract")) {
        mode = EXTRACT_PRETTY;
    }

    string output_format = "";
    for (const auto &s : args.get_positionals()) {
        if (s.empty()) output_format += ' ';
        output_format += s;
    }

    if (mode == EXTRACT_PRETTY && output_format.empty()) {
        cerr << "Missing output format." << endl;
        exit(1);
    }

    string value;

    auto positionals = args.get_positionals();

    Color col;
    string col_string, word, line;

    // clang-format off
    std::regex expr(
        "#([0-9a-f])([0-9a-f])([0-9a-f])\\b|"                                      // #RGB
        "#([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})\\b|"                             // #RRGGBB
        "rgb\\(\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*\\)|" // rgb(R, G, B)
        "hsl\\(\\s*([0-9]{1,3})\\s+([0-9]{1,3})\\s+([0-9]{1,3})\\s*\\)"            // hsl(H S L)
    );
    // clang-format on

    std::smatch sm;
    while (std::getline(cin, line)) {
        while (col.from_str(line, sm)) {
            if (mode == HIGHLIGHT) {
                cout << sm.prefix();
            }

            for (auto xf : xfs) {
                col.transform(xf);
            }
            for (auto mix : mixes) {
                col.mix_with(mix.that, mix.amount);
            }

            if (mode == EXTRACT_PRETTY) {
                col.pretty_print();
            }
            else if (mode == HIGHLIGHT) {
                cout << col.ansi_fg() << (output_format.empty() ? col.raw_string : col.printf(output_format)) << ANSI_RESET;
            }
            else {
                cout << col.printf(output_format);
            }

            if (mode == EXTRACT) {
                cout << '\n';
            }

            line = sm.suffix();
        }

        if (mode == HIGHLIGHT) {
            cout << line;
        }
    }

    return 0;
}
