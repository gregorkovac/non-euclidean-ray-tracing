#include "../include/Camera.h"

Camera::Camera(Vector position, Vector rotation, Vector scale, float f) : Object(position, rotation, scale) {
    this->f_ = f;
}

float Camera::f() {
    return this->f_;
}

bool Camera::intersect(Vector a, Vector b) {
    return false;
}

float Camera::equation(Vector v) {
    return 0;
}

float Camera::derivative(Vector v) {
    return 0;
}

Vector Camera::gradient(Vector v) {
    return Vector(0, 0, 0);
}