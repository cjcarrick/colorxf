#include <cmath>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using std::pow;
using std::string;

enum OperatorKind
{
    NULL_OPERATOR,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    NUMBER,
};

struct Token
{
    OperatorKind kind;
    double value;
};

class StrArithmetic
{
private:
    string str;
    static bool is_operator(char &c);
    static bool is_operator(char &c, OperatorKind &op);
    static void do_operation(double &lhs, OperatorKind &op, double &rhs);

    void tokenize();
    std::vector<Token> tokens;

    double read_number_forwards(int from);
    double read_number_backwards(int from);

public:
    StrArithmetic(string s) : str(s){};
    double calculate();
};

bool StrArithmetic::is_operator(char &c, OperatorKind &op)
{
    switch (c) {
    case '*': op = TIMES; return true;
    case '+': op = PLUS; return true;
    case '-': op = MINUS; return true;
    case '/': op = DIVIDE; return true;
    default: return false;
    }
}

// bool StrArithmetic::is_operator(char &c)
// {
//     switch (c) {
//     case '*':
//     case '+':
//     case '-':
//     case '/': return true;
//     default: return false;
//     }
// }

double StrArithmetic::read_number_forwards(int from)
{
    double total = 0;
    int frac = 0;
    for (int i = from; i < str.size() && str[i] >= '0' && str[i] <= '9'; i++) {
        total = total * (frac ? 10 : 1) +
                (str[i] - '0') * (frac + 1 ? pow(0.1, frac++) : 1);
    }
    return total;
}

double StrArithmetic::read_number_backwards(int from)
{
    double total = 0;
    int frac = 0;
    for (int i = from; i > 0 && str[i] >= '0' && str[i] <= '9'; i--) {
        total = total * (frac ? 10 : 1) +
                (str[i] - '0') * (frac + 1 ? pow(0.1, frac++) : 1);
    }
    return total;
}

void StrArithmetic::tokenize()
{
    double total = 0;
    int frac = 0;
    bool pending_num = false;

    for (int i = 0; i < str.size(); i++) {

        if (str[i] == '.') {
            pending_num = true;
            frac = 1;
            continue;
        }

        if (pending_num && (str[i] == '*' || str[i] == '+' || str[i] == '-' ||
                            str[i] == '/')) {
            tokens.push_back(Token{NUMBER, total});
            frac = total = pending_num = 0;
        }

        switch (str[i]) {
        case '*': tokens.push_back(Token{TIMES}); continue;
        case '+': tokens.push_back(Token{PLUS}); continue;
        case '-': tokens.push_back(Token{MINUS}); continue;
        case '/': tokens.push_back(Token{DIVIDE}); continue;
        }

        if (str[i] >= '0' && str[i] <= '9') {
            pending_num = true;
            if (frac) {
                total += (str[i] - '0') * pow(0.1, frac);
                frac++;
            }
            else {
                total *= 10;
                total += str[i] - '0';
            }
        }
    }

    if (pending_num) {
        tokens.push_back(Token{NUMBER, total});
    }

    for (auto t : tokens) {
        if (t.kind != NUMBER) continue;
        std::cout << t.value << std::endl;
    }
}

void StrArithmetic::do_operation(double &lhs, OperatorKind &op, double &rhs)
{
    switch (op) {
    case PLUS: lhs += rhs; break;
    case MINUS: lhs -= rhs; break;
    case TIMES: lhs *= rhs; break;
    case DIVIDE: lhs /= rhs; break;
    }
}

double StrArithmetic::calculate()
{
    tokenize();

    // Do * and / first
    for (int i = 0; i < tokens.size(); i++) {
        const auto &token = tokens[i];
        if (token.kind != TIMES && token.kind != DIVIDE) continue;
        do_operation(tokens[i - 1].value, tokens[i].kind, tokens[i + 1].value);
        tokens.erase(tokens.begin() + i, tokens.begin() + i + 1);
    }

    // Now do + and -
    for (int i = 0; i < tokens.size(); i++) {
        const auto &token = tokens[i];
        if (token.kind != PLUS && token.kind != MINUS) continue;

        std::cout << tokens[i - 1].value << std::endl;
        do_operation(tokens[i - 1].value, tokens[i].kind, tokens[i + 1].value);
        std::cout << "after math: " << tokens[i - 1].value << std::endl;

        tokens.erase(tokens.begin() + i, tokens.begin() + i + 1);
    }

    return tokens[0].value;
}

// double StrArithmetic::calculate()
// {
//
//
//
//     double total = 0;
//     double this_num = 0;
//     OperatorKind op = NULL_OPERATOR;
//     int frac = -1;
//     double add = 0;
//
//     for (auto c : str) {
//
//         if (c == '.') {
//             frac = 1;
//             continue;
//         }
//
//         if (is_operator(c)) {
//             if (op) {
//                 is_operator(c, op);
//                 std::cout << this_num << " " << total;
//                 this_num = 0;
//             }
//             else {
//                 is_operator(c, op);
//             }
//             continue;
//         }
//
//         if (c >= '0' && c <= '9') {
//             add = (c - '0') * (frac + 1 ? pow(0.1, frac++) : 1);
//             std::cout << "number" << std::endl;
//
//             if (op) {
//                 this_num = this_num * (frac ? 10 : 1) + add;
//             }
//             else {
//                 total = total * (frac ? 10 : 1) + add;
//             }
//         }
//     }
//
//     return total;
// }
