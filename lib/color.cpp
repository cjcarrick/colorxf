#include "./color.h"
#include "./util.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using std::max;
using std::min;
using std::setw;

#define ANSI_RESET "\e[0m"

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
    double d1, d2, d3;

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
        case 'a': ss << r; break;
        case 'b': ss << g; break;
        case 'c': ss << b; break;

        case 'A': ss << int(r * 255); break;
        case 'B': ss << int(g * 255); break;
        case 'C': ss << int(b * 255); break;

        case 'D': ss << int_to_str(r * 255, 16, 2); break;
        case 'E': ss << int_to_str(g * 255, 16, 2); break;
        case 'F': ss << int_to_str(b * 255, 16, 2); break;

        case 'g':
            hsl(&d1, &d2, &d3);
            ss << d1;
            break;
        case 'G':
            hsl(&d1, &d2, &d3);
            ss << int(d1 * 360);
            break;

        case 'h':
            hsl(&d1, &d2, &d3);
            ss << d2;
            break;
        case 'H':
            hsl(&d1, &d2, &d3);
            ss << int(d2 * 100);
            break;

        case 'i':
            hsl(&d1, &d2, &d3);
            ss << d3;
            break;
        case 'I':
            hsl(&d1, &d2, &d3);
            ss << int(d3 * 100);
            break;

        case 'j':
            hsv(&d1, &d2, &d3);
            ss << d2;
            break;
        case 'J':
            hsv(&d1, &d2, &d3);
            ss << int(d2 * 100);
            break;

        case 'k':
            hsv(&d1, &d2, &d3);
            ss << d3;
            break;
        case 'K':
            hsv(&d1, &d2, &d3);
            ss << int(d3 * 100);
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
Color::Color(int r, int g, int b) : r(r), g(g), b(b){};

string Color::ansi_fg() const
{
    return printf("\e[38;2;%A;%B;%Cm");
}
string Color::ansi_bg() const
{
    return printf("\e[48;2;%A;%B;%Cm");
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
    double rgb_r, rgb_g, rgb_b;
    rgb(&rgb_r, &rgb_g, &rgb_b);

    double hsl_h, hsl_s, hsl_l;
    hsl(&hsl_h, &hsl_s, &hsl_l);

    double hsv_h, hsv_s, hsv_v;
    hsv(&hsv_h, &hsv_s, &hsv_v);

    // clang-format off
    std::cout << ansi_bg() << "      " << ANSI_RESET << printf(" R:  %A \tH:  %G \tH:  %G") << '\n'
         << ansi_bg() << "      " << ANSI_RESET << printf(" G:  %B \tS:  %H \tS:  %J") << '\n'
         << ansi_bg() << "      " << ANSI_RESET << printf(" B:  %C \tL:  %I \tV:  %K") << '\n'
         << ""        << "      " << ""         << printf(" #%D%E%F")                   << '\n';
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

    double d1, d2, d3;

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
        d1 = min(max(handle_operation(d1, 100), 0.0), 1.0);
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
        d3 = min(max(handle_operation(d3, 100), 0.0), 1.0);
        hsv(d1, d2, d3);
        return;

    case transforms::HSV_V:
        hsv(&d1, &d2, &d3);
        d3 = min(max(handle_operation(d3, 100), 0.0), 1.0);
        hsv(d1, d2, d3);
        return;

    case transforms::NUL_CHANNEL: return;
    }
}
