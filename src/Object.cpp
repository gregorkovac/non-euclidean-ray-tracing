#include "../include/Object.h"

Object::Object(Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
    this->color_ = color;
    
    if (translucency > 1) this->translucency_ = 1;
    else if (translucency < 0) this->translucency_ = 0;
    else this->translucency_ = translucency;

    if (reflectivity > 1) this->reflectivity_ = 1;
    else if (reflectivity < 0) this->reflectivity_ = 0;
    else this->reflectivity_ = reflectivity;
}

Object::Object(Vector position, Vector rotation, Vector scale, Color color) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
    this->color_ = color;
    this->translucency_ = 0;
    this->reflectivity_ = 0;
}

Object::Object(Vector position, Vector rotation, Vector scale) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
    this->color_ = DEFAULT_OBJECT_COLOR;
    this->translucency_ = 0;
    this->reflectivity_ = 0;
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

float Object::translucency() {
    return this->translucency_;
}

float Object::reflectivity() {
    return this->reflectivity_;
}

Vector Object::newtonsMethod(Vector x0) {
    int i = 0;

    do {
        float f = this->equation(x0);
        Vector grad = this->gradient(x0);

        Vector x1 = x0 - (grad * f).normalize3();

        if (x1.distance(x0) < EPSILON) {
            return x1;
        }

        x0 = x1;
        i++;
    } while (i < MAX_ITER);

    return x0;
}