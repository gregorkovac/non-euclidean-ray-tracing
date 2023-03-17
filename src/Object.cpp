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
    this->color_ = DEFAULT_OBJECT_COLOR;
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

Vector Object::newtonsMethod(Vector x0) {
    int i = 0;

    do {
        float f = this->equation(x0);
        Vector grad = this->gradient(x0);

        Vector x1 = x0 - (grad * f).normalize();

        if (x1.distance(x0) < EPSILON) {
            return x1;
        }

        x0 = x1;
        i++;
    } while (i < MAX_ITER);

    return x0;
}