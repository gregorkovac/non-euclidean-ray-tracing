#ifndef RENDERE_H
#define RENDERE_H

#include "Misc.h"
#include "Object.h"
#include "ParametricObjects.h"
#include "Camera.h"

#define MAX_ITER 100
#define STEP_SIZE 0.02

const Color BACKGROUND_COLOR = {0, 0, 0};

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
        Color trace(Vector ray, Vector origin, int depth);
};

#endif