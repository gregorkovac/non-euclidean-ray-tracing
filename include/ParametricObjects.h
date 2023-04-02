#ifndef PARAMETRICOBJECTS_H
#define PARAMETRICOBJECTS_H

#include "Object.h"

class Sphere : public Object {
    private:
        float radius;

    public:
        Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color);
        Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex);
        Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType);
        bool intersect(Vector a, Vector b);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
        bool inSphere(Vector v);
};

class Plane : public Object {
    private:
        Vector normal_;

    public:
        Plane(Vector position, Vector rotation, Vector scale, Color color);
        Plane(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex);
        Plane(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType);
        bool intersect(Vector a, Vector b);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
};

#endif