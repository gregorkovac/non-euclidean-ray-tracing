#include "../include/ParametricObjects.h"

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType) {
    this->radius = radius;
    strcpy(this->type_, "Sphere");
}

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex) {
    this->radius = radius;
    strcpy(this->type_, "Sphere");
}

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->radius = radius;
    strcpy(this->type_, "Sphere");
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

bool Sphere::inSphere(Vector v) {
    if (v.distance(this->position_) <= this->radius * this->scale_.x)
        return true;
    return false;
}

Plane::Plane(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType) {
    this->normal_ = (Matrix::rotation(rotation) * Vector(0, 1, 0)).normalize();
    strcpy(this->type_, "Plane");
}

Plane::Plane(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex) {
    this->normal_ = (Matrix::rotation(rotation) * Vector(0, 1, 0)).normalize();
    strcpy(this->type_, "Plane");
}

Plane::Plane(Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->normal_ = (Matrix::rotation(rotation) * Vector(0, 1, 0)).normalize();
    strcpy(this->type_, "Plane");
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

Torus::Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType) {
    this->majorRadius = majorRadius;
    this->minorRadius = minorRadius;
    strcpy(this->type_, "Torus");
}

Torus::Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex) {
    this->majorRadius = majorRadius;
    this->minorRadius = minorRadius;
    strcpy(this->type_, "Torus");
}

Torus::Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->majorRadius = majorRadius;
    this->minorRadius = minorRadius;
    strcpy(this->type_, "Torus");
}

float Torus::equation(Vector v) {
    return pow(this->minorRadius - sqrt(pow(v.x - this->position_.x, 2) + pow(v.y - this->position_.y, 2)), 2) + pow(v.z - this->position_.z, 2) - pow(this->majorRadius, 2);
    //return pow(pow((v.x - this->position_.x), 2) + pow(v.y - this->position_.y, 2) + pow(v.z - this->position_.z, 2) - pow(this->majorRadius * this->scale_.x, 2), 2) - 4 * pow(this->minorRadius * this->scale_.x, 2) * (pow(v.x - this->position_.x, 2) + pow(v.z - this->position_.z, 2));
}

Vector Torus::gradient(Vector v) {
    return Vector(
        (v.x - this->position_.x) * (2 - (2 * this->minorRadius) / (sqrt(pow(v.x - this->position_.x, 2) + pow(v.y - this->position_.y, 2)))),
        (v.y - this->position_.y) * (2 - (2 * this->minorRadius) / (sqrt(pow(v.x - this->position_.x, 2) + pow(v.y - this->position_.y, 2)))),
        2 * (v.z - this->position_.z)
    );
}

Vector Torus::normal(Vector v) {
    return Vector(
        (v.x - this->position_.x) * (2 - (2 * this->minorRadius) / (sqrt(pow(v.x - this->position_.x, 2) + pow(v.y - this->position_.y, 2)))),
        (v.y - this->position_.y) * (2 - (2 * this->minorRadius) / (sqrt(pow(v.x - this->position_.x, 2) + pow(v.y - this->position_.y, 2)))),
        2 * (v.z - this->position_.z)
    ).normalize3();
}