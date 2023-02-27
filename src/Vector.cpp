#include <math.h>
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

void Vector::normalize() {
    this->x /= this->w;
    this->y /= this->w;
    this->z /= this->w;
    this->w = 1;
}