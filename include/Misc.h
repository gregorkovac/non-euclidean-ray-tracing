#ifndef MISC_H
#define MISC_H

struct Color {
    unsigned short r, g, b;
};

enum ColorType {
    COLOR_TYPE_SOLID,
    COLOR_TYPE_GRADIENT,
    COLOR_TYPE_CHECKERBOARD
};

int sign(float x);

#endif