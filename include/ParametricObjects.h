#ifndef PARAMETRICOBJECTS_H
#define PARAMETRICOBJECTS_H

#include "Object.h"

class Sphere : public Object {
    private:
        float radius;

    public:
        Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color);
        Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity);
        Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity, char* colorType);
        bool intersect(Vector a, Vector b);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
};

class Plane : public Object {
    private:
        Vector normal_;

    public:
        Plane(Vector position, Vector rotation, Vector scale, Color color);
        Plane(Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity);
        Plane(Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity, char* colorType);
        bool intersect(Vector a, Vector b);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
};

#endif