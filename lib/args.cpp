#include "./args.h"
#include "./color.h"
#include "iostream"
#include "string"
#include "vector"

using namespace std;

Args::Args(char **argv, int argc)
{
    bool need_value = false;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        // Skip empty args
        if (arg.length() == 0) {
            continue;
        }

        // If arg is 1 character long, it is either a value or a positional
        if (arg.length() == 1) {
            if (need_value) values.push_back(arg);
            else positionals.push_back(arg);
            need_value = false;
            continue;
        }

        // If arg begins with two dashes, it must be a flag
        if (arg[0] == '-' && arg[1] == '-') {
            keys.push_back(arg.substr(2));
            if (need_value) values.push_back("");
            need_value = true;
            continue;
        }

        // If arg begins with 1 dash and is 2 characters long, it must be a flag
        if (arg[0] == '-' && arg.size() == 2) {
            keys.push_back(arg.substr(1));
            if (need_value) values.push_back("");
            need_value = true;
            continue;
        }

        // Otherwise, it must be a positional
        if (need_value) values.push_back(arg);
        else positionals.push_back(arg);
        need_value = false;
    }
}

void Args::set_bool_opt(const string &key)
{
    int i = -1;
    for (int j = 0; j < keys.size(); j++) {
        if (keys[i] == key) {
            j = i;
            break;
        }
    }

    if (i == -1) return;

    positionals.push_back(values[i]);
    keys[i] = "";
}

bool Args::get(const string &key) const
{
    for (const auto &k : keys)
        if (k == key) return true;
    return false;
}

bool Args::get(const string &key, vector<string> &value) const
{
    for (int i = 0; i < keys.size(); i++) {
        if (keys[i] == key) {
            value.push_back(values[i]);
        }
    }
    return !value.empty();
}

bool Args::get(char key, vector<string> &value) const
{
    for (int i = 0; i < keys.size(); i++) {
        if (keys[i].length() != 1) continue;
        if (keys[i][0] == key) {
            value.push_back(values[i]);
        }
    }
    return !value.empty();
}

bool Args::get(char key) const
{
    for (const auto &k : keys) {
        if (k.length() != 1) continue;
        if (k[0] == key) return true;
    }
    return false;
}

bool Args::get(const string &key, string &value) const
{
    // Search for key in keys
    int i = -1;
    for (int j = 0; j < keys.size(); j++) {
        if (key == keys[j]) {
            i = j;
            break;
        }
    }

    // Could not find key
    if (i == -1) return false;

    value = values[i];
    return true;
}

bool Args::get(char key, string &value) const
{
    // Search for key in keys
    int i = -1;
    for (int j = 0; j < keys.size(); j++) {
        if (keys[j].size() > 1) continue;
        if (key == keys[j][0]) {
            i = j;
            break;
        };
    }

    // Could not find key
    if (i == -1) return false;

    value = values[i];
    return true;
}

vector<string> Args::get() const
{
    return keys;
}

vector<string> Args::get_positionals() const
{
    auto result = vector<string>();
    for (auto p : positionals) {
        if (p.empty()) continue;
        result.push_back(p);
    }
    return result;
}

enum CHAR_KIND
{
    LOWERCASE_LETTER,
    CAPITAL_LETTER,
    NUMBER,
    SYMBOL,
};

CHAR_KIND char_kind(char c)
{
    if (c >= 'a' && c <= 'z') return LOWERCASE_LETTER;
    if (c >= 'A' && c <= 'Z') return CAPITAL_LETTER;
    if (c >= '0' && c <= '9') return NUMBER;
    return SYMBOL;
}

void print_help()
{
    cout << R"("
Usage: colorxf [OPTIONS] [-x TRANSFORMS] [-m MIXES] OUTPUT
Read from stdin, capture COLORS, apply any TRANSFORMS, and output colors in format specified by OUTPUT.
Example: echo "My favorite color is #fe7 and #ee178a is not." | color "rgb(%A, %B, %C)\"

COLORS must be of the following format:
    #RGB
    #RRGGBB
    rgb(R, G, B)
    hsl(H S L)

Options:
    -e, --extract          Only output colors, each on a new line.
    -m, --mix              Mix with another color. See MIXES for details.
    -x, --transform        Apply transformations to input colors. See TRANSFORMS for detauls.
    -E, --extract-pretty   Only output colors, with rich information about each color. OUTPUT is ignored.
    -h, --help             Print help and exit.
    -v, --version          Print version and exit.

OUTPUT can be any string, with these special sequences:
    RGB
    %a    Red              0.00-1.00
    %A    Red              0-255 (integer)
    %b    Green            0.00-1.00
    %B    Green            0-255 (integer)
    %c    Blue             0.00-1.00
    %C    Blue             0-255 (integer)

    HEX
    %D    Red              00-ff
    %E    Green            00-ff
    %F    Blue             00-ff

    HSL
    %g    Hue              0.00-1.00
    %G    Hue              0-360 (integer)
    %h    Saturation       0.00-1.00
    %H    Saturation       0-100 (integer)
    %i    Lightness        0.00-1.00
    %I    Lightness        0-100 (integer)

    HSV
    %g    Hue              0.00-1.00
    %G    Hue              0-360 (integer)
    %j    Saturation       0.00-1.00
    %J    Saturation       0-100 (integer)
    %k    Value            0.00-1.00
    %K    Value            0-100 (integer)

    CMYK
    %o    Cyan             0.00-1.00
    %O    Cyan             0-100 (integer)
    %p    Magenta          0.00-1.00
    %P    Magenta          0-100 (integer)
    %q    Yellow           0.00-1.00
    %Q    Yellow           0-100 (integer)
    %r    Black            0.00-1.00
    %R    Black            0-100 (integer)

    %l    ANSI Truecolor FG Escape Sequence
    %m    ANSI Truecolor BG Escape Sequence

    %%    Literal percent sign

    Note that saturation and lightness are calculated differently in the HSL and HSV color models, but hue is the same.

    Any unrecognized sequences will be left as-is; '%z' will stay as '%z', etc.

TRANSFORMS is a string, structured as follows:
    <CHANNEL><OPRATION><VALUE>

    They are used to set, add to, or multiply color channels.

    Where CHANNEL is one of:
        rgb_r
        rgb_g
        rgb_b
        hsl_h
        hsl_s
        hsl_l
        hsv_h
        hsv_s
        hsv_v
        cmyk_c
        cmyk_m
        cmyk_y
        cmyk_k

    And OPERATION is one of:
        +
        -
        =
        *

    And VALUE is a number.

    -x can be specified more than once, to transform a color in multiple ways.

    Example:
        echo #eee | color -x "rgb_r=12" -x "rgb_g+84" -x "hsl_s*0.2" "#%D%E%F"

MIXES is a string, structured as follows:
    <COLOR>,<AMOUNT>

    Where COLOR is a valid color, and AMOUNT is a number from 0-1.

    -m can be specified more than once, to mix with more than one color.
)";
}
