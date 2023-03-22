#ifndef OBJECT_H
#define OBJECT_H

#include <math.h>

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
        float translucency_;
        float reflectivity_;

    public:
        Object(Vector position, Vector rotation, Vector scale, Color color, float translucency, float reflectivity);
        Object(Vector position, Vector rotation, Vector scale, Color color);
        Object(Vector position, Vector rotation, Vector scale);
        Vector position();
        Vector rotation();
        Vector scale();
        Vector forward();
        Vector right();
        Vector up();
        Color color();
        float translucency();
        float reflectivity();
        virtual bool intersect(Vector a, Vector b) = 0;
        virtual float equation(Vector v) = 0;
        virtual float derivative(Vector v) = 0;
        virtual Vector gradient(Vector v) = 0;
        virtual Vector normal(Vector v) = 0;
        Vector newtonsMethod(Vector x0);
};

#endif