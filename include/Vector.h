#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

class Vector {
    public:
        float x, y, z, w;

        Vector();
        Vector(float x, float y, float z);
        Vector(float x, float y, float z, float w);
        Vector(Vector *v);
        void normalize();
        Vector add(Vector v);
        Vector sub(Vector v);
        float dot(Vector v);
};

#endif