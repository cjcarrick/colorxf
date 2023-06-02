#include "./transforms.h"
#include "./util.h"
#include <regex>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace transforms
{

OPERATION parse_delimiter(string delimiter)
{
    if (delimiter == "*") return MULTIPLY;
    if (delimiter == "-") return SUBTRACT;
    if (delimiter == "+") return ADD;
    if (delimiter == "=") return SET;
    return NUL_OPERATION;
}

CHANNEL parse_channel(string channel)
{
    if (channel == "rgb_r") return RGB_R;
    if (channel == "rgb_g") return RGB_G;
    if (channel == "rgb_b") return RGB_B;
    if (channel == "hsl_h") return HSL_H;
    if (channel == "hsl_s") return HSL_S;
    if (channel == "hsl_l") return HSL_L;
    if (channel == "hsv_h") return HSV_H;
    if (channel == "hsv_s") return HSV_S;
    if (channel == "hsv_v") return HSV_V;
    if (channel == "cmyk_c") return CMYK_C;
    if (channel == "cmyk_m") return CMYK_M;
    if (channel == "cmyk_y") return CMYK_Y;
    if (channel == "cmyk_k") return CMYK_K;
    return NUL_CHANNEL;
}

vector<ColorTransform> from_str(string s)
{
    auto xf_strs = split(s, std::regex("\\s*,\\s*"));
    std::regex regexp(
        "([a-z_]+)([+-=*])([0-9.]+)", std::regex_constants::icase
    );
    std::smatch sm;

    vector<ColorTransform> xfs;
    ColorTransform xf;
    for (auto &xf_str : xf_strs) {
        if (!std::regex_search(xf_str, sm, regexp)) continue;
        xf.channel = parse_channel(sm[1].str()),
        xf.operation = parse_delimiter(sm[2].str()),
        xf.value = (int)parse_num(sm[3].str());
        xfs.push_back(xf);
    }
    return xfs;
}
} // namespace transforms
