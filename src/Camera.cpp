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