#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stdio.h>
#include "ParametersAndConstants.h"

class Vector {
    public:
        double x, y, z, w;

        Vector();
        Vector(double x, double y, double z);
        Vector(double x, double y, double z, double w);
        Vector(Vector *v);
        Vector normalize();
        Vector normalize3();
        Vector operator+(Vector v);
        Vector operator-(Vector v);
        double operator*(Vector v);
        Vector operator*(double s);
        Vector operator/(double s);
        bool operator==(Vector v);
        bool operator!=(Vector v);
        char* toString();
        double distance(Vector v);
        double norm();
        Vector cross(Vector v);
};

Vector operator*(double f, Vector v);
#endif