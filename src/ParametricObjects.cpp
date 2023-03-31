#include "../include/ParametricObjects.h"

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity, char* colorType) : Object(position, rotation, scale, color, translucency, reflectivity, colorType) {
    this->radius = radius;
}

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity) : Object(position, rotation, scale, color, translucency, reflectivity) {
    this->radius = radius;
}

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->radius = radius;
}

float Sphere::equation(Vector v) {
    return pow((v.x - this->position_.x), 2) + pow(v.y - this->position_.y, 2) + pow(v.z - this->position_.z, 2) - pow(this->radius * this->scale_.x, 2);
}

Vector Sphere::gradient(Vector v) {
    return Vector(2 * (v.x - this->position_.x), 2 * (v.y - this->position_.y), 2 * (v.z - this->position_.z));
}

Vector Sphere::normal(Vector v) {
    return Vector(v.x - this->position_.x, v.y - this->position_.y, v.z - this->position_.z);
}

Plane::Plane(Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity, char* colorType) : Object(position, rotation, scale, color, translucency, reflectivity, colorType) {
    this->normal_ = (Matrix::rotation(rotation) * Vector(0, 1, 0)).normalize();
}

Plane::Plane(Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity) : Object(position, rotation, scale, color, translucency, reflectivity) {
    this->normal_ = (Matrix::rotation(rotation) * Vector(0, 1, 0)).normalize();
}

Plane::Plane(Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->normal_ = (Matrix::rotation(rotation) * Vector(0, 1, 0)).normalize();
}

float Plane::equation(Vector v) {
    //printf("%s * (%s - %s) = %s * %s = %f\n", this->normal_.toString(), v.toString(), this->position_.toString(), this->normal_.toString(), vec.toString(), this->normal_ * (v - this->position_));
    return this->normal_ * (v - this->position_);
}

Vector Plane::gradient(Vector v) {
    return this->normal_;
}

Vector Plane::normal(Vector v) {
    return this->normal_;
}