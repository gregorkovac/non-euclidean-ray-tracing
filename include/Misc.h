#ifndef MISC_H
#define MISC_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
float randomBetween(float a, float b);

bool operator==(Color a, Color b);
bool operator!=(Color a, Color b);

#endif