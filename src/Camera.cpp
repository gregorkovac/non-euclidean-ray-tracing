#include "../include/Camera.h"

Camera::Camera(Vector position, Vector rotation, Vector scale, float f) : Object(position, rotation, scale) {
    this->f_ = f;
}

float Camera::f() {
    return this->f_;
}