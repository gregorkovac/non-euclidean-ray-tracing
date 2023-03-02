#include "../include/ParametricObjects.h"

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->radius = radius;
}

bool Sphere::intersect(Vector a, Vector b) {
    if (sign(this->equation(a)) != sign(this->equation(b)))
        return true;

    return false;
}

float Sphere::equation(Vector v) {
    return pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) - pow(this->radius, 2);
}