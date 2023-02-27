#ifndef PARAMETRICOBJECTS_H
#define PARAMETRICOBJECTS_H

#include "Object.h"

class Sphere : public Object {
    private:
        float radius;

    public:
        Sphere(float radius, Vector position, Vector rotation, Vector scale);
};

#endif