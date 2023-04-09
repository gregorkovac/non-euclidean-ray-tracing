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
        Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType, char* normalMap);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
        bool inSphere(Vector v);
        float u(Vector v);
        float v(Vector v);
};

class Plane : public Object {
    private:
        Vector normal_;

    public:
        Plane(Vector position, Vector rotation, Vector scale, Color color);
        Plane(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex);
        Plane(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType);
        Plane(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType, char* normalMap);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
        float u(Vector v);
        float v(Vector v);
};

class Torus : public Object {
    private:
        float majorRadius;
        float minorRadius;

    public:
        Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color);
        Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex);
        Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType);
        Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType, char* normalMap);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
        float u(Vector v);
        float v(Vector v);
};

class Hyperboloid : public Object {
    private:
        float a, b, c;

    public:
        Hyperboloid(float a, float b, float c, Vector position, Vector rotation, Vector scale, Color color);
        Hyperboloid(float a, float b, float c, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex);
        Hyperboloid(float a, float b, float c, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType);
        Hyperboloid(float a, float b, float c, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType, char* normalMap);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
        float u(Vector v);
        float v(Vector v);
};

#endif