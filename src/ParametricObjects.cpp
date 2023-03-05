#include "../include/ParametricObjects.h"

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->radius = radius;
}

bool Sphere::intersect(Vector a, Vector b) {
    //printf("%d %d\n", (int)sign(this->equation(a)), (int)sign(this->equation(b)));
    //printf("%s %s %f %f\n", a.toString(), b.toString(), this->equation(a), this->equation(b));
    if (sign(this->equation(a)) != sign(this->equation(b))) {
        return true;
    }

    return false;
}

float Sphere::equation(Vector v) {
    return pow((v.x - this->position_.x), 2) + pow(v.y - this->position_.y, 2) + pow(v.z - this->position_.z, 2) - pow(this->radius, 2);
}