#include "../include/ParametricObjects.h"

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale) : Object(position, rotation, scale) {
    this->radius = radius;
}