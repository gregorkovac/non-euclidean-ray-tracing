#include "../include/Camera.h"

Camera::Camera(Vector position, Vector rotation, Vector scale) : Object(position, rotation, scale) {
}

bool Camera::intersect(Vector a, Vector b) {
    return false;
}

float Camera::equation(Vector v) {
    return 0;
}

Vector Camera::gradient(Vector v) {
    return Vector(0, 0, 0);
}

Vector Camera::normal(Vector v) {
    return Vector(0, 0, 0);
}

float Camera::u(Vector v) {
    return 0;
}

float Camera::v(Vector v) {
    return 0;
}