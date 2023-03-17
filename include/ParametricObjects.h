#ifndef PARAMETRICOBJECTS_H
#define PARAMETRICOBJECTS_H

#include "Object.h"

class Sphere : public Object {
    private:
        float radius;

    public:
        Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color);
        bool intersect(Vector a, Vector b);
        float equation(Vector v);
        float derivative(Vector v);
        Vector gradient(Vector v);
};

#endif