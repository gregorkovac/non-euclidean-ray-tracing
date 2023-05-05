#include "../include/Vector.h"

Vector::Vector() {
    x = 0;
    y = 0;
    z = 0;
    w = 1;
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

Vector Vector::operator+(Vector v) {
    float x = this->x + v.x;
    float y = this->y + v.y;
    float z = this->z + v.z;
    float w = this->w + v.w;

    return Vector(x, y, z, w);
}

Vector Vector::operator-(Vector v) {
    float x = this->x - v.x;
    float y = this->y - v.y;
    float z = this->z - v.z;
    float w = this->w - v.w;

    return Vector(x, y, z, w);
}

float Vector::operator*(Vector v) {
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

Vector operator*(float f, Vector v) {
    return Vector(v.x * f, v.y * f, v.z * f);
}

Vector Vector::normalize() {
    float x = this->x / this->w;
    float y = this->y / this->w;
    float z = this->z / this->w;
    float w = 1;

    return Vector(x, y, z, w);
}

Vector Vector::normalize3() {
    float norm = sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));

    float x = this->x / norm;
    float y = this->y / norm;
    float z = this->z / norm;

    return Vector(x, y, z, 1);
}

Vector Vector::operator*(float s) {
    float x = this->x * s;
    float y = this->y * s;
    float z = this->z * s;
    float w = this->w * s;

    return Vector(x, y, z, w);
}

Vector Vector::operator/(float s) {
    float x = this->x / s;
    float y = this->y / s;
    float z = this->z / s;
    float w = this->w / s;

    return Vector(x, y, z, w);
}

char* Vector::toString() {
    char* str = new char[100];
    sprintf(str, "(%f, %f, %f, %f)", this->x, this->y, this->z, this->w);
    return str;
}

float Vector::distance(Vector v) {
    return sqrt(pow(this->x - v.x, 2) + pow(this->y - v.y, 2) + pow(this->z - v.z, 2));
}

float Vector::norm() {
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

bool Vector::operator==(Vector v) {
    return this->x == v.x && this->y == v.y && this->z == v.z;
}

bool Vector::operator!=(Vector v) {
    return this->x != v.x || this->y != v.y || this->z != v.z;
}

Vector Vector::cross(Vector v) {
    float x = this->y * v.z - this->z * v.y;
    float y = this->z * v.x - this->x * v.z;
    float z = this->x * v.y - this->y * v.x;

    return Vector(x, y, z);
}