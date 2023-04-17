#ifndef RENDERE_H
#define RENDERE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Misc.h"
#include "Object.h"
#include "ParametricObjects.h"
#include "Camera.h"
#include "ParametersAndConstants.h"
#include "Lights.h"

class Renderer {
    private:
        int frameWidth;
        int frameHeight;
        Camera* camera;
        Object** objects;
        Light** lights;
        int numObjects;
        int numLights;

    public:
        Renderer(int frameWidth, int frameHeight);
        void parseScene(char* sceneFilePath);
        void render(unsigned char* dataBuffer);
        Color trace(Vector ray, Vector origin, int depth);
        bool isShadowed(Vector origin, Vector light);
        UV rungeKutta4(UV x, UV y, float t);
        UV VectorToUV(Vector v);
        Vector UVToVector(UV uv);
};

#endif