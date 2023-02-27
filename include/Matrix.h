#ifndef MATRIX_H
#define MATRIX_H

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
};

#endif