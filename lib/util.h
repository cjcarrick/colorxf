#include <regex>
#include <string>
#include <vector>

using std::string;
using std::vector;

string int_to_str(
    int in,
    /** 10 for base-10, 16 for base-16, etc */
    int radix = 10,
    /** pads the beginning of the string with 0's */
    int min_length = 0
);

int parse_int(const string &num, int radix = 10);

double parse_num(string num);

/** Converts char ch to hexadecimal integer */
int hex_int(char ch);

vector<string> split(string &s, const std::regex &regexp);

