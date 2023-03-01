#include "../include/Vector.h"

Vector::Vector() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Vector::Vector(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 1;
}

Vector::Vector(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector::Vector(Vector *v) {
    this->x = v->x;
    this->y = v->y;
    this->z = v->z;
    this->w = v->w;
}

Vector Vector::add(Vector v) {
    float x = this->x + v.x;
    float y = this->y + v.y;
    float z = this->z + v.z;
    float w = this->w + v.w;

    return Vector(x, y, z, w);
}

Vector Vector::sub(Vector v) {
    float x = this->x - v.x;
    float y = this->y - v.y;
    float z = this->z - v.z;
    float w = this->w - v.w;

    return Vector(x, y, z, w);
}

float Vector::dot(Vector v) {
    return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w;
}

Vector Vector::normalize() {
    float x = this->x / this->w;
    float y = this->y / this->w;
    float z = this->z / this->w;
    float w = 1;

    return Vector(x, y, z, w);
}

Vector Vector::scalar(float s) {
    float x = this->x * s;
    float y = this->y * s;
    float z = this->z * s;
    float w = this->w * s;

    return Vector(x, y, z, w);
}