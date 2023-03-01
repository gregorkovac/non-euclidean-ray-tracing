#ifndef RENDERE_H
#define RENDERE_H

#include "Misc.h"
#include "Object.h"
#include "ParametricObjects.h"
#include "Camera.h"

class Renderer {
    private:
        int windowWidth;
        int windowHeight;
        float pixelSize;
        Camera* camera;
        Object** objects;
        
    public:
        Renderer(int windowWidth, int windowHeight, float pixelSize);
        void render(unsigned char* dataBuffer);
};

#endif