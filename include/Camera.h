#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"
#include "ParametersAndConstants.h"

class Camera : public Object {
    public:
        Camera(Vector position, Vector rotation, Vector scale);
        bool intersect(Vector a, Vector b);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
        float u(Vector v);
        float v(Vector v);
};

#endif