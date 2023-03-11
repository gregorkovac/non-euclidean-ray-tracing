#include "../include/Renderer.h"

#include <stdio.h>

Renderer::Renderer(int windowWidth, int windowHeight, float pixelSize) {
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->pixelSize = pixelSize;

    this->camera = new Camera(Vector(0, 0, 0), Vector(0, 0, 0), Vector(1, 1, 1), 1);

    this->objects = new Object*[2];
    this->lights = new Sphere*[1];

    Color red = {255, 0, 0};
    Color blue = {100, 255, 255};
    Color white = {255, 255, 255};

    this->objects[0] = new Sphere(1, Vector(0, 0, 2), Vector(0, 0, 0), Vector(1, 1, 1), red);
    this->objects[1] = new Sphere(1, Vector(-1, 0, 2), Vector(0, 0, 0), Vector(0.5, 0.5, 0.5), blue);

    this->lights[0] = new Sphere(1, Vector(-1, 1, 0), Vector(0, 0, 0), Vector(1, 1, 1), white);
}

void Renderer::render(unsigned char* dataBuffer) {
    Vector imagePlaneCenter = camera->position() + camera->forward() * camera->f();

    for (int y = 0; y < windowHeight; y++) {
        for (int x = 0; x < windowWidth; x++) {

            Vector imagePlanePoint = imagePlaneCenter + camera->right() * ((x - windowWidth / 2) * pixelSize) + camera->up() * ((y - windowHeight / 2) * pixelSize);
            Vector ray = imagePlanePoint - camera->position();
            
            Color color = trace(ray, camera->position(), 0);

            dataBuffer[(y * windowWidth + x) * 3] = color.r;
            dataBuffer[(y * windowWidth + x) * 3 + 1] = color.g;
            dataBuffer[(y * windowWidth + x) * 3 + 2] = color.b;
        }
    }
    
}

Color Renderer::trace(Vector ray, Vector origin, int depth) {
    Vector curr = origin;
    Vector prev = origin;

    for (float h = 0; h < MAX_ITER; h += 1) {
        prev = curr;
        curr = origin + ray * (h * STEP_SIZE);

        // TODO: Define scene in seperate file and then parse it. Take into acount the number of objects and use it here.
        for (int i = 0; i < 2; i++) {
            if (objects[i]->intersect(prev, curr)) {
                Color c = objects[i]->color();

                for (int j = 0; j < 1; j++)
                {
                    if (isShadowed(prev, lights[j]->position()))
                    {
                        c.r = c.r * 0.5;
                        c.g = c.g * 0.5;
                        c.b = c.b * 0.5;
                    }
                    else
                    {
                        float distToLight = prev.distance(lights[j]->position());

                        distToLight = distToLight > 2 ? 2 : distToLight;

                        c.r = c.r * (2 - distToLight);
                        c.g = c.g * (2 - distToLight);
                        c.b = c.b * (2 - distToLight);
                    }
                }

                return c;
            }
        }
    }

    return SKY_COLOR;
}

bool Renderer::isShadowed(Vector origin, Vector light)
{
    Vector curr = origin;
    Vector prev = origin;

    Vector ray = light - origin;

    for (float h = 0; h < MAX_ITER; h += 1)
    {
        prev = curr;
        curr = origin + ray * (h * STEP_SIZE);

        for (int i = 0; i < 2; i++)
        {
            if (objects[i]->intersect(prev, curr))
            {
                return true;
            }
        }
    }

    return false;
}