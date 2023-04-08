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
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
};

class Torus : public Object {
    private:
        float majorRadius;
        float minorRadius;

    public:
        Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color);
        Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex);
        Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
        float x(float u, float v);
        float y(float u, float v);
        float z(float u, float v);
        float xPartialU(float u, float v);
        float yPartialU(float u, float v);
        float zPartialU(float u, float v);
        float xPartialV(float u, float v);
        float yPartialV(float u, float v);
        float zPartialV(float u, float v);
        Vector F(float u, float v, float t, Vector U, Vector V);
        Matrix JF(float u, float v, float t, Vector U, Vector V);
        Vector solveParametric(Vector rayOrigin, Vector rayDirection, float rayStep);
        Vector newtonsMethod(Vector x0, Vector U, Vector V);
};

#endif