#include "../include/ParametricObjects.h"

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType) {
    this->radius = radius;
    strcpy(this->type_, "Sphere");
}

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex) {
    this->radius = radius;
    strcpy(this->type_, "Sphere");
}

Sphere::Sphere(float radius, Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->radius = radius;
    strcpy(this->type_, "Sphere");
}

float Sphere::equation(Vector v) {
    return pow((v.x - this->position_.x), 2) + pow(v.y - this->position_.y, 2) + pow(v.z - this->position_.z, 2) - pow(this->radius * this->scale_.x, 2);
}

Vector Sphere::gradient(Vector v) {
    return Vector(2 * (v.x - this->position_.x), 2 * (v.y - this->position_.y), 2 * (v.z - this->position_.z));
}

Vector Sphere::normal(Vector v) {
    return Vector(v.x - this->position_.x, v.y - this->position_.y, v.z - this->position_.z);
}

bool Sphere::inSphere(Vector v) {
    if (v.distance(this->position_) <= this->radius * this->scale_.x)
        return true;
    return false;
}

Plane::Plane(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType) {
    this->normal_ = (Matrix::rotation(rotation) * Vector(0, 1, 0)).normalize();
    strcpy(this->type_, "Plane");
}

Plane::Plane(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex) {
    this->normal_ = (Matrix::rotation(rotation) * Vector(0, 1, 0)).normalize();
    strcpy(this->type_, "Plane");
}

Plane::Plane(Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->normal_ = (Matrix::rotation(rotation) * Vector(0, 1, 0)).normalize();
    strcpy(this->type_, "Plane");
}

float Plane::equation(Vector v) {
    //printf("%s * (%s - %s) = %s * %s = %f\n", this->normal_.toString(), v.toString(), this->position_.toString(), this->normal_.toString(), vec.toString(), this->normal_ * (v - this->position_));
    return this->normal_ * (v - this->position_);
}

Vector Plane::gradient(Vector v) {
    return this->normal_;
}

Vector Plane::normal(Vector v) {
    return this->normal_;
}

Torus::Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType) {
    this->majorRadius = majorRadius;
    this->minorRadius = minorRadius;
    strcpy(this->type_, "Torus");
    this->isEquationParametric_ = true;
}

Torus::Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex) {
    this->majorRadius = majorRadius;
    this->minorRadius = minorRadius;
    strcpy(this->type_, "Torus");
    this->isEquationParametric_ = true;
}

Torus::Torus(float majorRadius, float minorRadius, Vector position, Vector rotation, Vector scale, Color color) : Object(position, rotation, scale, color) {
    this->majorRadius = majorRadius;
    this->minorRadius = minorRadius;
    strcpy(this->type_, "Torus");
    this->isEquationParametric_ = true;
}

float Torus::x (float u, float v) {
    return (this->majorRadius + this->minorRadius * cos(v)) * cos(u);
}

float Torus::xPartialU (float u, float v) {
    return -sin(u) * (this->majorRadius + this->minorRadius * cos(v));
}

float Torus::xPartialV (float u, float v) {
    return -this->minorRadius * sin(v) * cos(u);
}

float Torus::y (float u, float v) {
    return (this->majorRadius + this->minorRadius * cos(v)) * sin(u);
}

float Torus::yPartialU (float u, float v) {
    return cos(u) * (this->majorRadius + this->minorRadius * cos(v));
}

float Torus::yPartialV (float u, float v) {
    return -this->minorRadius * sin(v) * sin(u);
}

float Torus::z (float u, float v) {
    return this->minorRadius * sin(v);
}

float Torus::zPartialU (float u, float v) {
    return 0;
}

float Torus::zPartialV (float u, float v) {
    return this->minorRadius * cos(v);
}

Vector ray(Vector U, Vector V, float t) {
    return U + V * t;
}

Vector Torus::F(float u, float v, float t, Vector U, Vector V) {
    Vector r = ray(U, V, t);

    return Vector(
        this->x(u, v) - r.x,
        this->y(u, v) - r.y,
        this->z(u, v) - r.z
    );
}

Matrix Torus::JF(float u, float v, float t, Vector U, Vector V) {
    Vector rayPartialT = V;
    
    float m[4][4] = {
        {this->xPartialU(u, v), this->xPartialV(u, v), V.x, 0},
        {this->yPartialU(u, v), this->yPartialV(u, v), V.y, 0},
        {this->zPartialU(u, v), this->zPartialV(u, v), V.z, 0},
        {0, 0, 0, 0}};

    return Matrix(m);
}

Vector Torus::newtonsMethod(Vector x0, Vector U, Vector V) {

    int i = 0;

    do {
        Vector F = this->F(x0.x, x0.y, x0.z, U, V);
        Matrix JF = this->JF(x0.x, x0.y, x0.z, U, V);

        Vector x1 = x0 - (JF.inverse() * F);

        if (x1.distance(x0) < EPSILON) {
            x1.w = 1;
            return x1;
        }

        x0 = x1;
        i++;
    } while (i < MAX_ITER);

    return Vector(-1, -1, -1, -1);
}

Vector Torus::solveParametric(Vector rayOrigin, Vector rayDirection, float rayStep) {

    for (int i = 0; i < 10; i++) {
        // Set random initial approximations
        float u = randomBetween(0, 2 * PI);
        float v = randomBetween(0, 2 * PI);

        Vector x0 = Vector(u, v, rayStep);

        Vector approximation = this->newtonsMethod(x0, rayOrigin, rayDirection);

        if (approximation.w != -1)
            return approximation;
    }

    return Vector(-1, -1, -1, -1);
}

float Torus::equation(Vector v) {

    /*
    
    x = (c + a * cos(v)) * cos(u)
    y = (c + a * cos(v)) * sin(u)
    z = a * sin(v)
    
    */

    return pow(this->minorRadius - sqrt(pow(v.x - this->position_.x, 2) + pow(v.y - this->position_.y, 2)), 2) + pow(v.z - this->position_.z, 2) - pow(this->majorRadius, 2);
    //return pow(pow((v.x - this->position_.x), 2) + pow(v.y - this->position_.y, 2) + pow(v.z - this->position_.z, 2) - pow(this->majorRadius * this->scale_.x, 2), 2) - 4 * pow(this->minorRadius * this->scale_.x, 2) * (pow(v.x - this->position_.x, 2) + pow(v.z - this->position_.z, 2));
}

Vector Torus::gradient(Vector v) {
    return Vector(
        (v.x - this->position_.x) * (2 - (2 * this->minorRadius) / (sqrt(pow(v.x - this->position_.x, 2) + pow(v.y - this->position_.y, 2)))),
        (v.y - this->position_.y) * (2 - (2 * this->minorRadius) / (sqrt(pow(v.x - this->position_.x, 2) + pow(v.y - this->position_.y, 2)))),
        2 * (v.z - this->position_.z)
    );
}

Vector Torus::normal(Vector v) {
    return Vector(
        (v.x - this->position_.x) * (2 - (2 * this->minorRadius) / (sqrt(pow(v.x - this->position_.x, 2) + pow(v.y - this->position_.y, 2)))),
        (v.y - this->position_.y) * (2 - (2 * this->minorRadius) / (sqrt(pow(v.x - this->position_.x, 2) + pow(v.y - this->position_.y, 2)))),
        2 * (v.z - this->position_.z)
    ).normalize3();
}

