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