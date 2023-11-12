#include "../include/Vector.h"

Vector::Vector() {
    x = 0;
    y = 0;
    z = 0;
    w = 1;
}

Vector::Vector(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 1;
}

Vector::Vector(double x, double y, double z, double w) {
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
    double x = this->x + v.x;
    double y = this->y + v.y;
    double z = this->z + v.z;
    double w = this->w + v.w;

    return Vector(x, y, z, w);
}

Vector Vector::operator-(Vector v) {
    double x = this->x - v.x;
    double y = this->y - v.y;
    double z = this->z - v.z;
    double w = this->w - v.w;

    return Vector(x, y, z, w);
}

double Vector::operator*(Vector v) {
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

Vector operator*(double f, Vector v) {
    return Vector(v.x * f, v.y * f, v.z * f);
}

Vector Vector::normalize() {
    double x = this->x / this->w;
    double y = this->y / this->w;
    double z = this->z / this->w;
    double w = 1;

    return Vector(x, y, z, w);
}

Vector Vector::normalize3() {
    double norm = sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));

    double x = this->x / norm;
    double y = this->y / norm;
    double z = this->z / norm;

    return Vector(x, y, z, 1);
}

Vector Vector::operator*(double s) {
    double x = this->x * s;
    double y = this->y * s;
    double z = this->z * s;
    double w = this->w * s;

    return Vector(x, y, z, w);
}

Vector Vector::operator/(double s) {
    double x = this->x / s;
    double y = this->y / s;
    double z = this->z / s;
    double w = this->w / s;

    return Vector(x, y, z, w);
}

char* Vector::toString() {
    char* str = new char[100];
    sprintf(str, "(%f, %f, %f, %f)", this->x, this->y, this->z, this->w);
    return str;
}

double Vector::distance(Vector v) {
    return sqrt(pow(this->x - v.x, 2) + pow(this->y - v.y, 2) + pow(this->z - v.z, 2));
}

double Vector::norm() {
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

bool Vector::operator==(Vector v) {
    return this->x == v.x && this->y == v.y && this->z == v.z;
}

bool Vector::operator!=(Vector v) {
    return this->x != v.x || this->y != v.y || this->z != v.z;
}

Vector Vector::cross(Vector v) {
    double x = this->y * v.z - this->z * v.y;
    double y = this->z * v.x - this->x * v.z;
    double z = this->x * v.y - this->y * v.x;

    return Vector(x, y, z);
}