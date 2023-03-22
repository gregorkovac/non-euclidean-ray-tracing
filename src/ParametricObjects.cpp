#include "../include/ParametricObjects.h"

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity) : Object(position, rotation, scale, color, translucency, reflectivity) {
    this->radius = radius;
}

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
    return pow((v.x - this->position_.x), 2) + pow(v.y - this->position_.y, 2) + pow(v.z - this->position_.z, 2) - pow(this->radius * this->scale_.x, 2);
}

float Sphere::derivative(Vector v) {
    return 2 * (v.x - this->position_.x) + 2 * (v.y - this->position_.y) + 2 * (v.z - this->position_.z);
}

Vector Sphere::gradient(Vector v) {
    return Vector(2 * (v.x - this->position_.x), 2 * (v.y - this->position_.y), 2 * (v.z - this->position_.z));
}

Vector Sphere::normal(Vector v) {
    return Vector(v.x - this->position_.x, v.y - this->position_.y, v.z - this->position_.z);
}

Plane::Plane(Vector normalParam, Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity) : Object(position, rotation, scale, color, translucency, reflectivity) {
    this->normal_ = normalParam;
}

Plane::Plane(Vector normalParam, Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->normal_ = normalParam;
}

bool Plane::intersect(Vector a, Vector b) {
    if (sign(this->equation(a)) != sign(this->equation(b))) {
        return true;
    }

    return false;
}

float Plane::equation(Vector v) {
    return this->normal_.x * (v.x - this->position_.x) + this->normal_.y * (v.y - this->position_.y) + this->normal_.z * (v.z - this->position_.z);
}

float Plane::derivative(Vector v) {
    return this->normal_.x + this->normal_.y + this->normal_.z;
}

Vector Plane::gradient(Vector v) {
    return this->normal_;
}

Vector Plane::normal(Vector v) {
    return this->normal_;
}