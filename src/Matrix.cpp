#include "../include/Matrix.h"

Matrix::Matrix() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = 0;
}

Matrix::Matrix(float m[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            this->m[i][j] = m[i][j];
}

Matrix::Matrix(Matrix *m) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            this->m[i][j] = m->m[i][j];
}

Matrix Matrix::multiply(Matrix m) {
    Matrix res = Matrix();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                res.m[i][j] += this->m[i][k] * m.m[k][j];

    return res;
}

Matrix Matrix::multiply(float s) {
    Matrix res = Matrix();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res.m[i][j] = this->m[i][j] * s;

    return res;
}

Vector Matrix::multiply(Vector v) {
    float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
    float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
    float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
    float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;

    return Vector(x, y, z, w);
}

Matrix Matrix::add(Matrix m) {
    Matrix res = Matrix();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res.m[i][j] = this->m[i][j] + m.m[i][j];

    return res;
}

Matrix Matrix::sub(Matrix m) {
    Matrix res = Matrix();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res.m[i][j] = this->m[i][j] - m.m[i][j];

    return res;
}

Matrix Matrix::transpose() {
    Matrix res = Matrix();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res.m[i][j] = this->m[j][i];

    return res;
}