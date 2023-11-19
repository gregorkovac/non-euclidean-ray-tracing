#include "../include/Vector4.h"

Vector4::Vector4(double x, double y, double z, double w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4::Vector4(Vector4 *v) {
    this->x = v->x;
    this->y = v->y;
    this->z = v->z;
    this->w = v->w;
}

Vector4 Vector4::operator+(Vector4 v) {
    double x = this->x + v.x;
    double y = this->y + v.y;
    double z = this->z + v.z;
    double w = this->w + v.w;

    return Vector4(x, y, z, w);
}

Vector4 Vector4::operator-(Vector4 v) {
    double x = this->x - v.x;
    double y = this->y - v.y;
    double z = this->z - v.z;
    double w = this->w - v.w;

    return Vector4(x, y, z, w);
}

double Vector4::operator*(Vector4 v) {
    return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w;
}

Vector4 operator*(double f, Vector4 v) {
    return Vector4(v.x * f, v.y * f, v.z * f, v.w * f);
}

Vector4 Vector4::operator*(double s) {
    double x = this->x * s;
    double y = this->y * s;
    double z = this->z * s;
    double w = this->w * s;

    return Vector4(x, y, z, w);
}

Vector4 Vector4::operator/(double s) {
    double x = this->x / s;
    double y = this->y / s;
    double z = this->z / s;
    double w = this->w / s;

    return Vector4(x, y, z, w);
}

char* Vector4::toString() {
    char* str = new char[200];
    sprintf(str, "(%f %f %f %f)", this->x, this->y, this->z, this->w);
    return str;
}

double Vector4::norm() {

    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2) + pow(this->w, 2));
}

bool Vector4::operator==(Vector4 v) {
    return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;
}

bool Vector4::operator!=(Vector4 v) {
    return this->x != v.x || this->y != v.y || this->z != v.z || this->w != v.w;
}