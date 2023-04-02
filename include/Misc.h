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
int min(float a, float b);
int max(float a, float b);

bool operator==(Color a, Color b);

#endif