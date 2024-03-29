#include "../include/Misc.h"

int sign(float x)
{
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    else
        return 0;
}

int min(float a, float b)
{
    if (a < b)
        return a;
    return b;
}

int max(float a, float b)
{
    if (a < b)
        return b;
    return a;
}

float randomBetween(float a, float b)
{
    float s = rand() / (float)RAND_MAX;
    return a + s * (b - a);
}

bool operator==(Color a, Color b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

float mapToSpace(float x, float a, float b)
{
    float spaceSize = b - a;

    while (x < a)
        x += spaceSize;
    while (x > b)
        x -= spaceSize;

    return x;
}

float mapToFundamentalDomain(float x, float a, float b)
{
    double range = b - a;
    double quotient = (x - a) / range;
    double fractionalPart = quotient - floor(quotient);
    return a + fractionalPart * range;
}

UV VectorToUV(float x, float y, float z, float sphereRadius)
{
    UV uv;
    uv.u = acos(z / sphereRadius);
    uv.v = atan2(y, x);

    return uv;
}