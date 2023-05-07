#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "Vector.h"
#include "Matrix.h"
#include "Misc.h"
#include "ParametersAndConstants.h"

class Object {
    protected:
        char type_[20];
        Vector position_;
        Vector rotation_;
        Vector scale_;
        Color color_;
        ColorType colorType_;
        float reflectivity_;
        float translucency_;
        float refractiveIndex_;
        Matrix rotationMatrix;
        int textureWidth, textureHeight, textureChannels;
        float textureScaleX, textureScaleY;
        Color* textureData;
        bool hasNormalMap = false;
        Vector* normalMapData;

    public:
        Object(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType, char* normalMap);
        Object(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType);
        Object(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex);
        Object(Vector position, Vector rotation, Vector scale, Color color);
        Object(Vector position, Vector rotation, Vector scale);
        Vector position();
        Vector rotation();
        Vector scale();
        Vector forward();
        Vector right();
        Vector up();
        Color color(Vector p);
        Color color();
        float translucency();
        float reflectivity();
        virtual bool intersect(Vector a, Vector b);
        virtual float equation(Vector v) = 0;
        virtual Vector gradient(Vector v) = 0;
        virtual Vector normal(Vector v) = 0;
        Vector newtonsMethod(Vector x0);
        float refractiveCoefficient(Vector p);
        char* toString();
        char* type();
        virtual float u(Vector p) = 0;
        virtual float v(Vector p) = 0;
        void parseColorType(char* colorType);
        void parseNormalMap(char* normalMap);
};

#endif