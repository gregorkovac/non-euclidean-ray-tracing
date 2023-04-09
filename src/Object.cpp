#include "../include/Object.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

Object::Object(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType, char* normalMap) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
    this->color_ = color;
    
    if (translucency > 1) this->translucency_ = 1;
    else if (translucency < 0) this->translucency_ = 0;
    else this->translucency_ = translucency;

    if (reflectivity > 1) this->reflectivity_ = 1;
    else if (reflectivity < 0) this->reflectivity_ = 0;
    else this->reflectivity_ = reflectivity;

    this->refractiveIndex_ = refractiveIndex;

    parseColorType(colorType);
    parseNormalMap(normalMap);
}

Object::Object(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
    this->color_ = color;
    
    if (translucency > 1) this->translucency_ = 1;
    else if (translucency < 0) this->translucency_ = 0;
    else this->translucency_ = translucency;

    if (reflectivity > 1) this->reflectivity_ = 1;
    else if (reflectivity < 0) this->reflectivity_ = 0;
    else this->reflectivity_ = reflectivity;

    this->refractiveIndex_ = refractiveIndex;

    parseColorType(colorType);
}

void Object::parseColorType(char* colorType) {
    if (strcmp(colorType, "solid") == 0) this->colorType_ = COLOR_TYPE_SOLID;
    else if (strcmp(colorType, "gradient") == 0) this->colorType_ = COLOR_TYPE_GRADIENT;
    else if (strcmp(colorType, "checkerboard") == 0) this->colorType_ = COLOR_TYPE_CHECKERBOARD;
    else if (colorType[0] == 't') {
        this->colorType_ = COLOR_TYPE_TEXTURE;
    
        char* texturePath = new char[strlen(colorType) - 8];
        sscanf(colorType, "texture-%f-%f-%s", &this->textureScaleX, &this->textureScaleY, texturePath);

        unsigned char* imageData = stbi_load(texturePath, &this->textureWidth, &this->textureHeight, &this->textureChannels, 0);

        this->textureData = new Color[this->textureWidth * this->textureHeight];

        for (int i = 0; i < this->textureWidth * this->textureHeight; i++) {
            this->textureData[i].r = imageData[i * 3];
            this->textureData[i].g = imageData[i * 3 + 1];
            this->textureData[i].b = imageData[i * 3 + 2];
        }

        stbi_image_free(imageData);
    }
    else this->colorType_ = COLOR_TYPE_SOLID;
}

void Object::parseNormalMap(char* normalMap) {
    if (strcmp(normalMap, "normalmap-none") == 0) {
        this->hasNormalMap = false;
        return;
    }

    this->hasNormalMap = true;

    char* normalMapPath = new char[strlen(normalMap) - 10];
    sscanf(normalMap, "normalmap-%s", normalMapPath);        

    int normalMapWidth, normalMapHeight, normalMapChannels;

    unsigned char* imageData = stbi_load(normalMapPath, &normalMapWidth, &normalMapHeight, &normalMapChannels, 0);

    this->normalMapData = new Vector[this->textureWidth * this->textureHeight];

    for (int i = 0; i < this->textureWidth * this->textureHeight; i++) {
        this->normalMapData[i].x = imageData[i * 3] / 255.0f * 2 - 1;
        this->normalMapData[i].y = imageData[i * 3 + 1] / 255.0f * 2 - 1;
        this->normalMapData[i].z = imageData[i * 3 + 2] / 255.0f * 2 - 1;
    }

    stbi_image_free(imageData);
}

Object::Object(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
    this->color_ = color;
    
    if (translucency > 1) this->translucency_ = 1;
    else if (translucency < 0) this->translucency_ = 0;
    else this->translucency_ = translucency;

    if (reflectivity > 1) this->reflectivity_ = 1;
    else if (reflectivity < 0) this->reflectivity_ = 0;
    else this->reflectivity_ = reflectivity;

    this->refractiveIndex_ = refractiveIndex;
}

Object::Object(Vector position, Vector rotation, Vector scale, Color color) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
    this->color_ = color;
    this->translucency_ = 0;
    this->reflectivity_ = 0;
    this->refractiveIndex_ = 1;
}

Object::Object(Vector position, Vector rotation, Vector scale) {
    this->position_ = position;
    this->rotation_ = rotation;
    this->scale_ = scale;
    this->color_ = DEFAULT_OBJECT_COLOR;
    this->translucency_ = 0;
    this->reflectivity_ = 0;
    this->refractiveIndex_ = 1;
}

Vector Object::position() {
    return this->position_;
}

Vector Object::rotation() {
    return this->rotation_;
}

Vector Object::scale() {
    return this->scale_;
}

Vector Object::forward() {
    return Matrix::rotation(this->rotation_) * Vector(0, 0, 1, 1);
}

Vector Object::right() {
    return Matrix::rotation(this->rotation_) * Vector(1, 0, 0, 1);
}

Vector Object::up() {
    return Matrix::rotation(this->rotation_) * Vector(0, 1, 0, 1);
}

Color Object::color(Vector p) {

    switch (colorType_) {
        case COLOR_TYPE_SOLID:
            return this->color_;
        case COLOR_TYPE_GRADIENT: 
            return {
                (unsigned short) (this->color_.r * (1 - p.z) + 255 * p.z),
                (unsigned short) (this->color_.g * (1 - p.z) + 255 * p.z),
                (unsigned short) (this->color_.b * (1 - p.z) + 255 * p.z)
            };
        case COLOR_TYPE_TEXTURE:
        {
            /*int u = (int)((this->position_.x - p.x) * 1000) % this->textureWidth;
            int v = (int)((this->position_.y - p.y) * 1000) % this->textureHeight;


            if (u < 0) u += this->textureWidth;
            if (v < 0) v += this->textureHeight; */

            int u = (int)(this->u(p) * this->textureScaleX) % this->textureWidth;
            int v = (int)(this->v(p) * this->textureScaleY) % this->textureHeight;

            if (u < 0) u += this->textureWidth;
            if (v < 0) v += this->textureHeight;

            return this->textureData[u + v * this->textureWidth];
        }

        case COLOR_TYPE_CHECKERBOARD:
        {
            // Map the point to plane 2d space
            float u = this->position_.x - p.x;
            float v = this->position_.y - p.y;

            // Map the point to a checkerboard
            int i = (int) (u * 5);
            int j = (int) (v * 5);

            // Return the color
            if ((i + j) % 2 == 0) {
                return this->color_;
            } else {
                return {
                    (unsigned short) (this->color_.r * 0.5),
                    (unsigned short) (this->color_.g * 0.5),
                    (unsigned short) (this->color_.b * 0.5)
                };
            }
        }
    }

    return MISSING_COLOR;
}

Color Object::color() {
    return this->color_;
}

float Object::translucency() {
    return this->translucency_;
}

float Object::reflectivity() {
    return this->reflectivity_;
}

Vector Object::newtonsMethod(Vector x0) {
    int i = 0;

    do {
        float f = this->equation(x0);
        Vector grad = this->gradient(x0);

        Vector x1 = x0 - (grad / pow(grad.norm(), 2) * f);

        if (x1.distance(x0) < EPSILON) {
            return x1;
        }

        x0 = x1;
        i++;
    } while (i < MAX_ITER);

    return x0;
}

float Object::refractiveCoefficient(Vector p) { 
    if (this->equation(p) < 0) {
        return AIR_REFRACTIVE_INDEX / this->refractiveIndex_;
    }
    
    return this->refractiveIndex_ / AIR_REFRACTIVE_INDEX;
}

bool Object::intersect(Vector a, Vector b) {
    if (sign(this->equation(a)) != sign(this->equation(b))) 
        return true;

    return false;
}

char* Object::toString() {
    char buffer[5000];

    sprintf(buffer, "%s: [ position: %s, rotation: %s, scale: %s, color: (%d, %d, %d), translucency: %f, reflectivity: %f ]",
        typeid(*this).name(),
        this->position_.toString(),
        this->rotation_.toString(),
        this->scale_.toString(),
        this->color_.r,
        this->color_.g,
        this->color_.b,
        this->translucency_,
        this->reflectivity_
    );

    return buffer;
}

char* Object::type() {
    return this->type_;
}