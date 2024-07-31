#include "./color.h"
#include "./util.h"
#include <cmath>
#include <iomanip>
#include <ios>
#include <iostream>
#include <regex>
#include <sstream>

using std::max;
using std::min;
using std::setw;

string Color::printf(const string &format) const
{
    string result;
    printf(format, result);
    return result;
}

void Color::printf(const string &format, string &result) const
{
    std::stringstream ss;
    bool interpret = false;
    double d1, d2, d3, d4;

    for (auto c : format) {
        if (c == '%') {
            interpret = true;
            continue;
        }

        if (!interpret) {
            ss << c;
            continue;
        }

        interpret = false;
        switch (c) {
        case 'a':
            ss << std::fixed << std::setprecision(2) << r;
            break;
        case 'b':
            ss << std::fixed << std::setprecision(2) << g;
            break;
        case 'c':
            ss << std::fixed << std::setprecision(2) << b;
            break;

        case 'A': ss << int(r * 255); break;
        case 'B': ss << int(g * 255); break;
        case 'C': ss << int(b * 255); break;

        case 'D': ss << int_to_str(r * 255, 16, 2); break;
        case 'E': ss << int_to_str(g * 255, 16, 2); break;
        case 'F': ss << int_to_str(b * 255, 16, 2); break;

        case 'g':
            hsl(&d1, &d2, &d3);
            ss << std::fixed << std::setprecision(2) << d1;
            break;
        case 'G':
            hsl(&d1, &d2, &d3);
            ss << int(d1 * 360);
            break;

        case 'h':
            hsl(&d1, &d2, &d3);
            ss << std::fixed << std::setprecision(2) << d2;
            break;
        case 'H':
            hsl(&d1, &d2, &d3);
            ss << int(d2 * 100);
            break;

        case 'i':
            hsl(&d1, &d2, &d3);
            ss << std::fixed << std::setprecision(2) << d3;
            break;
        case 'I':
            hsl(&d1, &d2, &d3);
            ss << int(d3 * 100);
            break;

        case 'j':
            hsv(&d1, &d2, &d3);
            ss << std::fixed << std::setprecision(2) << d2;
            break;
        case 'J':
            hsv(&d1, &d2, &d3);
            ss << int(d2 * 100);
            break;

        case 'k':
            hsv(&d1, &d2, &d3);
            ss << std::fixed << std::setprecision(2) << d3;
            break;
        case 'K':
            hsv(&d1, &d2, &d3);
            ss << int(d3 * 100);
            break;

        case 'o':
            cmyk(&d1, &d2, &d3, &d4);
            ss << std::fixed << std::setprecision(2) << d1;
            break;
        case 'O':
            cmyk(&d1, &d2, &d3, &d4);
            ss << int(d1 * 100);
            break;

        case 'p':
            cmyk(&d1, &d2, &d3, &d4);
            ss << std::fixed << std::setprecision(2) << d2;
            break;
        case 'P':
            cmyk(&d1, &d2, &d3, &d4);
            ss << int(d2 * 100);
            break;

        case 'q':
            cmyk(&d1, &d2, &d3, &d4);
            ss << std::fixed << std::setprecision(2) << d3;
            break;
        case 'Q':
            cmyk(&d1, &d2, &d3, &d4);
            ss << int(d3 * 100);
            break;

        case 'r':
            cmyk(&d1, &d2, &d3, &d4);
            ss << std::fixed << std::setprecision(2) << d4;
            break;
        case 'R':
            cmyk(&d1, &d2, &d3, &d4);
            ss << int(d4 * 100);
            break;

        case 'l': ss << ansi_fg(); break;
        case 'm': ss << ansi_bg(); break;

        case '%': ss << '%'; break;

        default: ss << '%' << c;
        }
    }

    result = ss.str();
}

Color::Color() : r(0), g(0), b(0){};
Color::Color(double r, double g, double b) : r(r), g(g), b(b){};

string Color::ansi_fg() const
{
    return printf("\e[38;2;%A;%B;%Cm");
}
string Color::ansi_bg() const
{
    return printf("\e[48;2;%A;%B;%Cm");
}

void Color::cmyk(double c, double m, double y, double k)
{
    r = (1.0 - c) * (1.0 - k);
    g = (1.0 - m) * (1.0 - k);
    b = (1.0 - y) * (1.0 - k);
}

void Color::cmyk(double *c, double *m, double *y, double *k) const
{
    *k = 1 - max({*c, *m, *y});
    *c = (1 - r - *k) / (1 - *k);
    *m = (1 - g - *k) / (1 - *k);
    *y = (1 - b - *k) / (1 - *k);
}

void Color::rgb(double _r, double _g, double _b)
{
    r = _r;
    g = _g;
    b = _b;
}

void Color::rgb(double *_r, double *_g, double *_b) const
{
    *_r = r;
    *_g = g;
    *_b = b;
}

void Color::hsv(double h, double s, double v)
{
    int i;
    double f, p, q, t;
    if (s == 0) {
        // achromatic (grey)
        r = g = b = v;
        return;
    }
    h *= 6;    // sector 0 to 5
    i = floor(h);
    f = h - i; // factorial part of h
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    switch (i) {
    case 0:
        r = v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = t;
        g = p;
        b = v;
        break;
    default: // case 5:
        r = v;
        g = p;
        b = q;
        break;
    }
}

void Color::hsv(double *h, double *s, double *v) const
{
    double mn, mx, delta;
    mn = min({r, g, b});
    mx = max({r, g, b});
    *v = mx;                      // v
    delta = mx - mn;
    if (mx != 0) *s = delta / mx; // s
    else {
        // r = g = b = 0		// s = 0, v is undefined
        *s = 0;
        *h = -1;
        return;
    }
    if (r == mx) *h = (g - b) / delta;          // between yellow & magenta
    else if (g == mx) *h = 2 + (b - r) / delta; // between cyan & yellow
    else *h = 4 + (r - g) / delta;              // between magenta & cyan
    *h /= 6;                                    // degrees
    if (*h < 0) *h += 1;
}

void Color::hsl(double *h, double *s, double *l) const
{
    double mx = max({r, g, b}), mn = min({r, g, b});
    *h = *s = *l = (mx + mn) / 2;

    if (mx == mn) {
        *h = *s = 0; // achromatic
    }
    else {
        double d = mx - mn;
        *s = *l > 0.5 ? d / (2 - mx - mn) : d / (mx + mn);
        if (mx == r) *h = (g - b) / d + (g < b ? 6 : 0);
        else if (mx == g) *h = (b - r) / d + 2;
        else if (mx == b) *h = (r - g) / d + 4;
        *h /= 6;
    }
}

void Color::hsl(double h, double s, double l)
{
    if (s == 0) {
        r = g = b = l; // achromatic
    }
    else {
        auto hue_to_rgb = [](double p, double q, double t)
        {
            if (t < 0.0) t += 1.0;
            if (t > 1.0) t -= 1.0;
            if (t < 1.0 / 6.0) return p + (q - p) * 6.0 * t;
            if (t < 1.0 / 2.0) return q;
            if (t < 2.0 / 3.0) return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
            return p;
        };

        if (s == 0.0) {
            r = g = b = l; // achromatic
        }
        else {
            double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            double p = 2 * l - q;
            r = hue_to_rgb(p, q, h + 1.0 / 3.0);
            g = hue_to_rgb(p, q, h);
            b = hue_to_rgb(p, q, h - 1.0 / 3.0);
        }
    }
}

void Color::pretty_print() const
{
    // clang-format off
    std::cout << printf("%m      ") << ANSI_RESET << "  R: " << setw(9) << printf("%A   H: ") << setw(9) << printf("%G   H: ") << setw(9) << printf("%G   C: ") << setw(4) << printf("%O") << std::endl
              << printf("%m      ") << ANSI_RESET << "  G: " << setw(9) << printf("%B   S: ") << setw(9) << printf("%H   S: ") << setw(9) << printf("%H   M: ") << setw(4) << printf("%P") << std::endl
              << printf("%m      ") << ANSI_RESET << "  B: " << setw(9) << printf("%C   L: ") << setw(9) << printf("%I   V: ") << setw(9) << printf("%I   Y: ") << setw(4) << printf("%Q") << std::endl
              <<          "      "  << ""  << printf("  #%D%E%F") << "                    "                                                           << "K: "  << setw(4) << printf("%R") << std::endl;
    // clang-format on
}

void Color::transform(transforms::ColorTransform xf)
{
    auto handle_operation = [xf]<typename T>(T n, double mult)
    {
        switch (xf.operation) {
        case transforms::ADD: n += xf.value / mult; break;
        case transforms::SUBTRACT: n -= xf.value / mult; break;
        case transforms::MULTIPLY: n *= xf.value; break;
        case transforms::SET: n = xf.value / mult; break;
        default: break;
        }
        return n;
    };

    double d1, d2, d3, d4;

    switch (xf.channel) {

    case transforms::RGB_R:
        r = min(max(handle_operation(r, 255), 0.0), 1.0);
        return;
    case transforms::RGB_G:
        g = min(max(handle_operation(g, 255), 0.0), 1.0);
        return;
    case transforms::RGB_B:
        b = min(max(handle_operation(b, 255), 0.0), 1.0);
        return;

    case transforms::HSL_H:
    case transforms::HSV_H:
        hsl(&d1, &d2, &d3);
        d1 = min(max(handle_operation(d1, 360), 0.0), 1.0);
        hsl(d1, d2, d3);
        return;

    case transforms::HSL_S:
        hsl(&d1, &d2, &d3);
        d2 = min(max(handle_operation(d2, 100), 0.0), 1.0);
        hsl(d1, d2, d3);
        return;

    case transforms::HSL_L:
        hsl(&d1, &d2, &d3);
        d3 = min(max(handle_operation(d3, 100), 0.0), 1.0);
        hsl(d1, d2, d3);
        return;

    case transforms::HSV_S:
        hsv(&d1, &d2, &d3);
        d2 = min(max(handle_operation(d3, 100), 0.0), 1.0);
        hsv(d1, d2, d3);
        return;

    case transforms::HSV_V:
        hsv(&d1, &d2, &d3);
        d3 = min(max(handle_operation(d3, 100), 0.0), 1.0);
        hsv(d1, d2, d3);
        return;

    case transforms::CMYK_C:
        cmyk(&d1, &d2, &d3, &d4);
        d1 = min(max(handle_operation(d1, 100), 0.0), 1.0);
        cmyk(d1, d2, d3, d4);
        return;

    case transforms::CMYK_M:
        cmyk(&d1, &d2, &d3, &d4);
        d2 = min(max(handle_operation(d2, 100), 0.0), 1.0);
        cmyk(d1, d2, d3, d4);
        return;

    case transforms::CMYK_Y:
        cmyk(&d1, &d2, &d3, &d4);
        d3 = min(max(handle_operation(d3, 100), 0.0), 1.0);
        cmyk(d1, d2, d3, d4);
        return;

    case transforms::CMYK_K:
        cmyk(&d1, &d2, &d3, &d4);
        d4 = min(max(handle_operation(d4, 100), 0.0), 1.0);
        cmyk(d1, d2, d3, d4);
        return;

    case transforms::NUL_CHANNEL:
        std::cout << " null channel "
                  << "\n";
        return;
    }
}

void Color::mix_with(Color &other, double amount)
{
    r = (1 - amount) * r + amount * other.r;
    g = (1 - amount) * g + amount * other.g;
    b = (1 - amount) * b + amount * other.b;
}

bool Color::from_str(string s)
{
    std::smatch sm;
    return from_str(s, sm);
}

bool Color::from_str(string &s, std::smatch &sm)
{
    // clang-format off
    std::regex expr(
        "#([0-9a-f])([0-9a-f])([0-9a-f])\\b|"                                      // #RGB
        "#([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})\\b|"                             // #RRGGBB
        "rgb\\(\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*\\)|" // rgb(R, G, B)
        "hsl\\(\\s*([0-9]{1,3})\\s+([0-9]{1,3})\\s+([0-9]{1,3})\\s*\\)"            // hsl(H S L)
    );
    // clang-format on

    if (std::regex_search(s, sm, expr)) {

        // Found #RRGGBB format
        if (sm[4].matched) {
            rgb(double(parse_int(sm[4].str(), 16)) / 255.0,
                double(parse_int(sm[5].str(), 16)) / 255.0,
                double(parse_int(sm[6].str(), 16)) / 255.0);
            raw_string = '#' + sm[4].str() + sm[5].str() + sm[6].str();
            return true;
        }

        // Found rgb(R, G, B) format
        if (sm[7].matched) {
            rgb(double(parse_num(sm[7])) / 255.0,
                double(parse_num(sm[8])) / 255.0,
                double(parse_num(sm[9])) / 255.0);
            raw_string = "rgb(" + sm[7].str() + ',' + sm[8].str() + ',' + sm[9].str() + ')';
            return true;
        }

        // Found #RGB format
        if (sm[1].matched) {
            rgb(double(
                    parse_int(sm[1].str(), 16) + parse_int(sm[1].str(), 16) * 16
                ) / 255.0,
                double(
                    parse_int(sm[2].str(), 16) + parse_int(sm[2].str(), 16) * 16
                ) / 255.0,
                double(
                    parse_int(sm[3].str(), 16) + parse_int(sm[3].str(), 16) * 16
                ) / 255.0);
                raw_string = '#' + sm[1].str() + sm[2].str() + sm[3].str();
            return true;
        }

        // Found hsl(H S L) format
        if (sm[10].matched) {
            hsl(parse_num(sm[10].str()) / 360.0,
                parse_num(sm[11].str()) / 100.0,
                parse_int(sm[12].str()) / 100.0);
            raw_string = "hsl(" + sm[10].str() + ' ' + sm[11].str() + ' ' + sm[12].str() + ')';
            return true;
        }
    }
    return false;
}
