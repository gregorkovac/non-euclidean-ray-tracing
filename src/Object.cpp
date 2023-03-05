#include "../include/Object.h"

Object::Object(Vector position, Vector rotation, Vector scale, Color color) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
    this->color_ = color;
}

Object::Object(Vector position, Vector rotation, Vector scale) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
    this->color_ = DEFAULT_COLOR;
}

Vector Object::position() {
    return this->position_;
}

Vector Object::rotation() {
    return this->rotation_;
}

Vector Object::scale() {
    return this->scale_;
}

Vector Object::forward() {
    return Matrix::rotation(this->rotation_) * Vector(0, 0, 1, 1);
}

Vector Object::right() {
    return Matrix::rotation(this->rotation_) * Vector(1, 0, 0, 1);
}

Vector Object::up() {
    return Matrix::rotation(this->rotation_) * Vector(0, 1, 0, 1);
}

Color Object::color() {
    return this->color_;
}