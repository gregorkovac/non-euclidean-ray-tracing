#ifndef OBJECT_H
#define OBJECT_H

#include "Vector.h"
#include "Matrix.h"

class Object {
    private:
        Vector position_;
        Vector rotation_;
        Vector scale_;

    public:
        Object(Vector position, Vector rotation, Vector scale);
        Vector position();
        Vector rotation();
        Vector scale();
        Vector forward();
        Vector right();
        Vector up();
};

#endif