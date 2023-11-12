#ifndef DOPRI_H
#define DOPRI_H

#include <vector>
#include <math.h>
#include "Vector.h"

using namespace std;

vector<Vector> DOPRI5 (Vector (*f)(Vector), double x0, Vector y0, double b, double epsilon, double sigma);

#endif