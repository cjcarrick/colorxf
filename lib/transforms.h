
#include <regex>
#include <string>
#include <vector>

using std::string;
using std::vector;

#ifndef TRANSFORMS_H
#define TRANSFORMS_H

namespace transforms
{

enum CHANNEL
{
    NUL_CHANNEL,
    RGB_R,
    RGB_G,
    RGB_B,
    HSL_H,
    HSL_S,
    HSL_L,
    HSV_H,
    HSV_S,
    HSV_V,
    CMYK_C,
    CMYK_M,
    CMYK_Y,
    CMYK_K,
};

enum OPERATION
{
    NUL_OPERATION,
    ADD,
    SUBTRACT,
    MULTIPLY,
    SET,
};

struct ColorTransform
{
    CHANNEL channel;
    OPERATION operation;
    int value;
};

OPERATION parse_delimiter(string delimiter);

CHANNEL parse_channel(string channel);

void from_str(string s, vector<ColorTransform>& output);

} // namespace transforms

#endif
