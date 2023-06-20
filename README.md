# colorxf

```
Usage: colorxf [OPTIONS] [TRANSFORMS] [MIXES] OUTPUT
Read from stdin, capture COLORS, apply any TRANSFORMS, and output colors in format specified by OUTPUT.
Example: echo "My favorite color is #fe7 and #ee178a is not." | color "rgb(%A, %B, %C)\"

COLORS must be of the following format:
    #RGB
    #RRGGBB
    rgb(R, G, B)
    hsl(H S L)

Options:
    -e, --extract          Only output colors, each on a new line.
    -m, --mix              Mix with another color. See MIXES for details.
    -E, --extract-pretty   Only output colors, with rich information about each color. OUTPUT is ignored.
    -h, --help             Print help and exit.
    -v, --version          Print version and exit.

OUTPUT can be any string, with these special sequences:
    RGB
    %a    Red              0.00-1.00
    %A    Red              0-255 (integer)
    %b    Green            0.00-1.00
    %B    Green            0-255 (integer)
    %c    Blue             0.00-1.00
    %C    Blue             0-255 (integer)

    HEX
    %D    Red              00-ff
    %E    Green            00-ff
    %F    Blue             00-ff

    HSL
    %g    Hue              0.00-1.00
    %G    Hue              0-360 (integer)
    %h    Saturation       0.00-1.00
    %H    Saturation       0-100 (integer)
    %i    Lightness        0.00-1.00
    %I    Lightness        0-100 (integer)

    HSV
    %g    Hue              0.00-1.00
    %G    Hue              0-360 (integer)
    %j    Saturation       0.00-1.00
    %J    Saturation       0-100 (integer)
    %k    Value            0.00-1.00
    %K    Value            0-100 (integer)

    CMYK
    %o    Cyan             0.00-1.00
    %O    Cyan             0-100 (integer)
    %p    Magenta          0.00-1.00
    %P    Magenta          0-100 (integer)
    %q    Yellow           0.00-1.00
    %Q    Yellow           0-100 (integer)
    %r    Black            0.00-1.00
    %R    Black            0-100 (integer)

    %l    ANSI Truecolor FG Escape Sequence
    %m    ANSI Truecolor BG Escape Sequence

    %%    Literal percent sign

    Note that saturation and lightness are calculated differently in the HSL and HSV color models, but hue is the same.

    Any unrecognized sequences will be left as-is; '%z' will stay as '%z', etc.

TRANSFORMS is a string, structured as follows:
    <CHANNEL><OPRATION><VALUE>[,<CHANNEL><OPERATION><VALUE>...]

    They are used to set, add to, or multiply color channels.

    Where CHANNEL is one of:
        rgb_r
        rgb_g
        rgb_b
        hsl_h
        hsl_s
        hsl_l
        hsv_h
        hsv_s
        hsv_v
        cmyk_c
        cmyk_m
        cmyk_y
        cmyk_k

    And OPERATION is one of:
        +
        -
        =
        *

    And VALUE is a number.

    Example:
        echo #eee | color "rgb_r=12,rgb_g+84,hsl_s*0.2" "#%D%E%F"

MIXES is a string, structured as follows:
    <COLOR>,<AMOUNT>

    Where COLOR is a valid color, and AMOUNT is a number from 0-1.

    -m can be specified more than once, to mix with more than one color.
```

## Building

```sh
make
```

Binary file is written to the project root.

## TODO

_None of these were a huge priority for me so I didn't add them._

- [ ] Add support for alpha channel
- [x] Add CMYK
