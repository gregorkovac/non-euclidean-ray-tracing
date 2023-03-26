#ifndef RENDERE_H
#define RENDERE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Misc.h"
#include "Object.h"
#include "ParametricObjects.h"
#include "Camera.h"
#include "ParametersAndConstants.h"

class Renderer {
    private:
        int frameWidth;
        int frameHeight;
        Camera* camera;
        Object** objects;
        Sphere** lights;
        int numObjects;
        int numLights;

    public:
        Renderer(int frameWidth, int frameHeight);
        void parseScene(char* sceneFilePath);
        void render(unsigned char* dataBuffer);
        Color trace(Vector ray, Vector origin, int depth);
        bool isShadowed(Vector origin, Vector light);
};

#endif