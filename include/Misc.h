#ifndef MISC_H
#define MISC_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Color {
    unsigned short r, g, b;
};

struct UV {
    double u, v;
};

enum ColorType {
    COLOR_TYPE_SOLID,
    COLOR_TYPE_GRADIENT,
    COLOR_TYPE_CHECKERBOARD,
    COLOR_TYPE_TEXTURE
};

int sign(float x);
int min(float a, float b);
int max(float a, float b);
float randomBetween(float a, float b);

bool operator==(Color a, Color b);

float mapToSpace(float x, float a, float b);
float mapToFundamentalDomain(float x, float a, float b);

enum SpaceType {
    EUCLIDEAN,
    FLAT_TORUS,
    MIRRORED_CUBE,
    SPHERICAL
};

UV VectorToUV(float x, float y, float z, float sphereRadius);

#endif