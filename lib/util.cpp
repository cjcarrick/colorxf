#include <cmath>
#include <regex>
#include <string>
#include <vector>

using std::string;
using std::vector;

int parse_int(const string &num, int radix = 10)
{
    if (radix == 0 || radix > 36) return -1;
    char first_char = '0';
    char last_char = "0123456789abcdefghijklmnopqrstuvwxyz"[radix];

    int result = 0;
    for (auto c : num) {
        if (c > last_char || c < first_char) return -1;
        if (c > '9' && c < 'a') return -1;
        if (c >= 'a') {
            result = (result * radix) + (c - 'a' + 10);
        }
        else {
            result = (result * radix) + (c - '0');
        }
    }

    return result;
}

double parse_num(string num)
{
    double n = 0;
    int dec_start = -1;
    for (int i = 0; i < num.size(); i++) {
        char c = num[i];

        // Cannot have more than one decimal point in a number
        if (c == '.' && dec_start > -1) {
            return -1;
        }
        if (c == '.') {
            dec_start = i;
            continue;
        }

        // Chars must be in range 0-9
        if (c < '0' || c > '9') {
            return -1;
        }

        // If we are adding decimal digits
        if (dec_start > -1) {
            n += double(c - '0') / pow(10.0, double(i - dec_start));
        }
        else {
            n = (n * 10) + (c - '0');
        }
    }
    return n;
}

string int_to_str(int in, int radix = 10, int min_length = 1)
{
    string result;
    char c;

    if (in) {
        for (int i = radix; i <= in * radix; i *= radix) {
            c = in % i / (i / radix);
            c += c > 9 ? 'a' - 10 : '0';
            result = c + result;
        }
    }

    if (result.length() < min_length) {
        result.insert(0, min_length - result.length(), '0');
    }

    return result;
}

int hex_int(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    return -1;
}

vector<string> split(string &s, const std::regex &regexp)
{
    vector<string> result;

    std::smatch sm;
    while (std::regex_search(s, sm, regexp)) {
        result.push_back(sm.prefix());
        s = sm.suffix();
    }
    result.push_back(s);

    return result;
}
