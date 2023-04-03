#include "Lights.h"

Light::Light(Vector position, Color color, float intensity) : Sphere(0, position, Vector(0,0,0), Vector(1,1,1), color) {
    this->intensity_ = intensity;
}

float Light::intensity() {
    return this->intensity_;
}