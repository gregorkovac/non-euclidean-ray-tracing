#ifndef DOPRI_H
#define DOPRI_H

#include <vector>
#include <math.h>
#include "Vector.h"
#include "Vector4.h"

using namespace std;

Vector4 F_spherical(Vector4 x);
vector<Vector4> DOPRI5 (Vector4 (*f)(Vector4), double x0, Vector4 y0, double b, double epsilon, double sigma);
Vector4 DOPRI5_one_step (Vector4 (*f)(Vector4), double x0, Vector4 y0, double *b, double epsilon, double sigma);

#endif