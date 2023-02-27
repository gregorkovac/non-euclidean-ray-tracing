#include "../include/Object.h"

Object::Object(Vector position, Vector rotation, Vector scale) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
}