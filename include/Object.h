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

    public:
        Object(Vector position, Vector rotation, Vector scale, Color color);
        Object(Vector position, Vector rotation, Vector scale);
        Vector position();
        Vector rotation();
        Vector scale();
        Vector forward();
        Vector right();
        Vector up();
        Color color();
        virtual bool intersect(Vector a, Vector b) = 0;
};

#endif