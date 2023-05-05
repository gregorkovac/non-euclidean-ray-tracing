#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stdio.h>
#include "ParametersAndConstants.h"

class Vector {
    public:
        float x, y, z, w;

        Vector();
        Vector(float x, float y, float z);
        Vector(float x, float y, float z, float w);
        Vector(Vector *v);
        Vector normalize();
        Vector normalize3();
        Vector operator+(Vector v);
        Vector operator-(Vector v);
        float operator*(Vector v);
        Vector operator*(float s);
        Vector operator/(float s);
        bool operator==(Vector v);
        bool operator!=(Vector v);
        char* toString();
        float distance(Vector v);
        float norm();
        Vector cross(Vector v);
};

Vector operator*(float f, Vector v);

#endif