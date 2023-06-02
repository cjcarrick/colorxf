#include "./lib/args.h"
#include "./lib/color.h"
#include "./lib/mix.h"
#include "./lib/transforms.h"
#include "./lib/util.h"
#include <cmath>
#include <cstdio>
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
    args.set_bool_opt("extract");
    args.set_bool_opt("extract-pretty");

    vector<string> mix_strs;
    args.get("m", mix_strs);
    args.get("mix", mix_strs);
    vector<mix::MixData> mixes;
    for (auto mix_str : mix_strs) mix::from_str(mix_str, mixes);

    if (args.get("help") || args.get('h')) {
        print_help();
        exit(0);
    }

    if (args.get("version") || args.get('v')) {
        cout << "0.3.0" << endl;
        exit(0);
    }

    bool extract_pretty = args.get('E') || args.get("extract-pretty");
    bool extract = !extract_pretty && (args.get('e') || args.get("extract"));

    string output_format;
    if (!extract_pretty &&
        (!args.get('o', output_format) && !args.get("output", output_format))) {
        cerr << "Missing output format." << endl;
        exit(1);
    }

    string value;

    auto positionals = args.get_positionals();

    auto xfs = positionals.empty() ? vector<transforms::ColorTransform>()
                                   : transforms::from_str(positionals[0]);

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

            if (!extract && !extract_pretty) {
                cout << sm.prefix();
            }

            for (auto xf : xfs) {
                col.transform(xf);
            }
            for (auto mix : mixes) {
                col.mix_with(mix.that, mix.amount);
            }

            if (extract_pretty) {
                col.pretty_print();
            }
            else {
                col.printf(output_format, col_string);
                cout << col_string;
            }
            if (extract) cout << '\n';

            line = sm.suffix();
        }

        if (!extract && !extract_pretty) cout << line;
    }

    return 0;
}
