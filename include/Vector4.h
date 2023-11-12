#ifndef VECTOR4_H
#define VECTOR4_H

#include <math.h>
#include <stdio.h>
#include "ParametersAndConstants.h"

class Vector4 {
    public:
        double x, y, z, w;

        Vector4(double x, double y, double z, double w);
        Vector4(Vector4 *v);
        Vector4 operator+(Vector4 v);
        Vector4 operator-(Vector4 v);
        double operator*(Vector4 v);
        Vector4 operator*(double s);
        Vector4 operator/(double s);
        bool operator==(Vector4 v);
        bool operator!=(Vector4 v);
        char* toString();
        double norm();
};

Vector4 operator*(double f, Vector4 v);

#endif