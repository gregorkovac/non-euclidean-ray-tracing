#ifndef RENDERE_H
#define RENDERE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <chrono>

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
        std::__1::chrono::steady_clock::time_point executionStart;

    public:
        Renderer(int frameWidth, int frameHeight);
        void parseScene(char* sceneFilePath);
        void render(unsigned char* dataBuffer);
        Color trace(Vector ray, Vector origin, int depth, int maxIter = MAX_ITER, float* distanceTravelled = NULL, Color* unlitColor = NULL);
        bool isShadowed(Vector origin, Vector light, Vector ray);
        UV rungeKutta4(UV x, UV y, float t);
        UV VectorToUV(Vector v);
        Vector UVToVector(UV uv);
        void initExecutionTime();
};

#endif