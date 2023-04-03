#ifndef LIGHTS_H
#define LIGHTS_H

#include "ParametricObjects.h"

class Light : public Sphere {
    private :
        float intensity_;

    public:
        Light(Vector position, Color color, float intensity);
        float intensity();
};

#endif