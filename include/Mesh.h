#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Object.h"
#include "Camera.h"

struct Triangle {
    Vector a, b, c;
    Vector normal;
};

class Mesh : public Object {
    private:
        int numTriangles;
        Triangle triangles[10000];
        int lastIntersectionTriangleIndex;

    public:
        Mesh(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType, char* objFilePath);
        void parseObjFile(char* objFilePath);
        float equation(Vector v);
        Vector gradient(Vector v);
        Vector normal(Vector v);
        float u(Vector v);
        float v(Vector v);
        void cullBackFaces(Camera* camera);
};

#endif