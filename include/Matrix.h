#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include "Vector.h"
#include "ParametersAndConstants.h"

class Matrix {
    float m[4][4];

    public:
        Matrix();
        Matrix(float m[4][4]);
        Matrix(Matrix *m);
        Matrix operator*(Matrix m);
        Matrix operator*(float s);
        Vector operator*(Vector v);
        Matrix operator+(Matrix m);
        Matrix operator-(Matrix m);
        Matrix transpose();

        static Matrix identity();
        static Matrix translation(Vector v);
        static Matrix rotationX(float angle);
        static Matrix rotationY(float angle);
        static Matrix rotationZ(float angle);
        static Matrix rotation(Vector v);
        static Matrix scale(Vector v);
};

#endif