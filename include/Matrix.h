#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include "Vector.h"

class Matrix {
    float m[4][4];

    public:
        Matrix();
        Matrix(float m[4][4]);
        Matrix(Matrix *m);
        Matrix multiply(Matrix m);
        Matrix multiply(float s);
        Vector multiply(Vector v);
        Matrix add(Matrix m);
        Matrix sub(Matrix m);
        Matrix transpose();

        static Matrix identity();
        static Matrix translation(Vector v);
        static Matrix rotationX(float angle);
        static Matrix rotationY(float angle);
        static Matrix rotationZ(float angle);
        static Matrix scale(Vector v);
};

#endif