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

/**
 * Parses the argument in "input", assuming it is in the following format:
 *
 * --key=value
 * --key+value
 * --key+=value
 * -k
 * --key
 *
 *  Returns true if the arg was parsed successfully, false otherwise
 *
 * The delimiter can be any string that doesn't have letters.
 *
 */
bool parse_arg(string &input, string &key, string &delimiter, string &value)
{
    if (input.length() < 2) return false;

    if (input.length() == 2) {
        if (input[0] == '-') {
            key = input[1];
            return true;
        }
    }

    if (input[0] != '-' && input[1] != '-') return false;

    enum LOCATION
    {
        KEY,
        DELIMITER,
        VALUE
    };

    LOCATION location = KEY;
    key = "";
    for (char c : input.substr(2)) {

        if (location == VALUE) {
            value += c;
            continue;
        }

        switch (char_kind(c)) {

        case SYMBOL:
            switch (location) {
            case KEY:
                location = DELIMITER;
                delimiter = c;
                break;
            case DELIMITER: delimiter += c; break;
            case VALUE: value += c; break;
            }
            break;
        default:
            switch (location) {
            case KEY: key += c; break;
            case DELIMITER:
                location = VALUE;
                value = c;
                break;
            case VALUE:
                // We should never hit this case because of the
                // if (location == VALUE)
                // defined above.
                value += c;
            }
        }
    }
    return true;
}

void print_help()
{
    cout << R"("
Usage: color [OPTIONS] [TRANSFORMS]... --output OUTPUT
Read words from stdin, capture colors, apply any TRANSFORMS, and output colors in format OUTPUT.
Example: echo my favorite color is #fff and #ee178a is not. | color -o "rgb(%A, %B, %C)\"

"Words" are whitespace-separated strings.

"Colors" must be of the following format:
    #RGB
    #RRGGBB
    #RRGGBBAA

Options:
    -e, --extract          Only output colors, each on a new line.
    -E, --extract-pretty   Only output colors, with rich information about each color. -o is ignored.
    -h, --help             Print help and exit.

OUTPUT can be any string, with these special sequences:
    RGB
    %a    Red                 0.00-1.00
    %A    Red                 0-255 (integer)
    %b    Green               0.00-1.00
    %B    Green               0-255 (integer)
    %c    Blue                0.00-1.00
    %C    Blue                0-255 (integer)

    HEX
    %D    Red                 00-ff
    %E    Green               00-ff
    %F    Blue                00-ff

    HSL
    %g    Hue                 0.00-1.00
    %G    Hue                 0-360 (integer)
    %h    Saturation          0.00-1.00
    %H    Saturation          0-100 (integer)
    %i    Lightness           0.00-1.00
    %I    Lightness           0-100 (integer)

    HSV
    %g    Hue                 0.00-1.00
    %G    Hue                 0-360 (integer)
    %j    Saturation          0.00-1.00
    %J    Saturation          0-100 (integer)
    %k    Value               0.00-1.00
    %K    Value               0-100 (integer)

    %l    ANSI Truecolor FG Escape Sequence
    %m    ANSI Truecolor BG Escape Sequence

    %%    Literal percent sign

    Note that saturation and lightness are calculated differently in the
    HSL and HSV color models, but hue is the same.

    Any unrecognized sequences will be left as-is; '%z' will stay as '%z',
    etc.

TRANSFORMS is a string, structured as follows:
    <CHANNEL><OPRATION><VALUE>[,<CHANNEL><OPERATION><VALUE>...]

    They are used to set, add to, or multiply color channels.

    Where CHANNEL is one of:
        red
        green
        blue
        hue
        saturation
        value

    And OPERATION is one of:
        +
        -
        =
        *

    And VALUE is a number.

    Example:
        echo #eee | color "red=12,green+84,saturation*0.2" -o "#%D%E%F"
")";
}
