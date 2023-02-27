#ifndef OBJECT_H
#define OBJECT_H

#include "Vector.h"

class Object {
    private:
        Vector position_;
        Vector rotation_;
        Vector scale_;

    public:
        Object(Vector position, Vector rotation, Vector scale);
        void move(Vector v);
        void rotate(Vector v);
        void scale(Vector v);
        void scale(float s);
};

#endif