#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "Vector.h"
#include "Matrix.h"
#include "Misc.h"
#include "ParametersAndConstants.h"

class Object {
    protected:
        Vector position_;
        Vector rotation_;
        Vector scale_;
        Color color_;
        ColorType colorType_;
        float reflectivity_;
        float translucency_;
        float refractiveIndex_;

    public:
        Object(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType);
        Object(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex);
        Object(Vector position, Vector rotation, Vector scale, Color color);
        Object(Vector position, Vector rotation, Vector scale);
        Vector position();
        Vector rotation();
        Vector scale();
        Vector forward();
        Vector right();
        Vector up();
        Color color(Vector p);
        float translucency();
        float reflectivity();
        bool intersect(Vector a, Vector b);
        virtual float equation(Vector v) = 0;
        virtual Vector gradient(Vector v) = 0;
        virtual Vector normal(Vector v) = 0;
        Vector newtonsMethod(Vector x0);
        float refractiveCoefficient(Vector p);
        char* toString();
};

#endif