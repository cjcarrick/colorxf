#include "./transforms.h"
#include "string"

using std::string;

#ifndef COLOR_H
#define COLOR_H

/**
 * All getters and setters will use doubles that range from 0-1
 */

class Color
{
private:
    double r;
    double g;
    double b;

public:
    /** an empty initializer will initialize the color to black */
    Color();
    Color(int r, int g, int b);

    /** Set HSL */
    void hsl(double h, double s, double l);
    /** Get HSL */
    void hsl(double *h, double *s, double *l) const;

    /** Set RGB */
    void rgb(double r, double g, double b);
    /** Get RGB */
    void rgb(double *r, double *g, double *b) const;

    /** Set HSV */
    void hsv(double h, double s, double v);
    /** Get HSV */
    void hsv(double *h, double *s, double *v) const;

    /**
     * Apply a transform to this color.
     *
     * If a channel or operation is invalid, nothing will happen.
     */
    void transform(transforms::ColorTransform xf);

    /** Attempts to interpret HEX, RGB, and HSL strings */
    bool from_str(string s);
    bool from_str(string &s, std::smatch &sm);

    // Printing {{

    /** See the print_help() function for detauls. */
    void printf(const string &format, string &result) const;
    [[nodiscard]] string printf(const string &format) const;

    void pretty_print() const;

    /** returns ansi escape sequence to begin drawing with this color as
     * foreground */
    [[nodiscard]] string ansi_fg() const;

    /** returns ansi escape sequence to begin drawing with this color as
     * background */
    [[nodiscard]] string ansi_bg() const;

    // }}
};

/**
 * Converts character to its hex value, assuming char is in the range 0-f.
 *
 * Returns -1 if out of range.
 */
int hex_int(char ch);

enum ColorType
{
    RGB,
    HSL,
    HEX,
    INVALID
};

#endif
