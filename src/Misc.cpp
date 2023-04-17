#include "../include/Misc.h"

int sign(float x) {
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    else
        return 0;
}

int min(float a, float b) {
    if (a < b)
        return a;
    return b; 
}

int max(float a, float b) {
    if (a < b)
        return b;
    return a;
}

float randomBetween(float a, float b) {
    int randomNumber = rand() % ((int)b - (int)a + 1);
    return randomNumber + a;
}

bool operator==(Color a, Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

float mapToSpace(float x, float a, float b) {
    float spaceSize = b - a;

    while (x < a)
        x += spaceSize;
    while (x > b)
        x -= spaceSize;

    return x;
}