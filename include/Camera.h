#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"

class Camera : public Object {
    private:
        float f_;

    public:
        Camera(Vector position, Vector rotation, Vector scale, float f);
        float f();
};

#endif