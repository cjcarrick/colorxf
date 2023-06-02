#include "./color.h"
#include <vector>

using std::vector;

namespace mix
{
struct MixData
{
    double amount;
    Color that;
};

void from_str(string s, vector<MixData>& mixes);
} // namespace mix
