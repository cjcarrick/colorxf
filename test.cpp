#include "./lib/str_arithmetic.cpp"
#include <iomanip>
#include <ios>
#include <iostream>

using std::cout;

int main()
{
    StrArithmetic s("89+1");
    cout << s.calculate() << std::endl;

    StrArithmetic s3("89+1+2");
    cout << std::showpoint << std::setprecision(3) << s3.calculate()
         << std::endl;

    StrArithmetic s2("89+1*2");
    cout << s2.calculate() << std::endl;

    return 0;
}
