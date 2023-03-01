#include "../include/Camera.h"

Camera::Camera(Vector position, Vector rotation, Vector scale, float fov) : Object(position, rotation, scale) {
    this->fov = fov;
}