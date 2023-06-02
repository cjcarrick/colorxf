#include "./mix.h"
#include "./color.h"
#include "./util.h"
#include <vector>

using std::vector;

namespace mix
{
void from_str(string s, vector<MixData>& mixes)
{
    std::regex regexp("(.*),([0-9.]+)", std::regex_constants::icase);
    std::smatch sm;

    Color col;
    MixData mix{0, col};

    if (!std::regex_search(s, sm, regexp)) return;
    if (!mix.that.from_str(sm[1].str())) return;
    mix.amount = parse_num(sm[2].str());

    mixes.push_back(mix);
}
} // namespace mix
