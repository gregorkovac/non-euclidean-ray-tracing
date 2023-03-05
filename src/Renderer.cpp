#include "../include/Renderer.h"

#include <stdio.h>

Renderer::Renderer(int windowWidth, int windowHeight, float pixelSize) {
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->pixelSize = pixelSize;

    this->camera = new Camera(Vector(0, 0, 0), Vector(0, 0, 0), Vector(1, 1, 1), 1);

    this->objects = new Object*[1];

    Color red = {255, 0, 0};
    this->objects[0] = new Sphere(1, Vector(0, 0, 10), Vector(0, 0, 0), Vector(1, 1, 1), red);
}

void Renderer::render(unsigned char* dataBuffer) {
    Vector imagePlaneCenter = camera->position() + camera->forward() * camera->f();

    // printf("Camera position: %s\n", camera->position().toString());
    // printf("Image plane center: %s\n", imagePlaneCenter.toString());

    for (int y = 0; y < windowHeight; y++) {
        for (int x = 0; x < windowWidth; x++) {

            Vector imagePlanePoint = imagePlaneCenter + camera->right() * ((x - windowWidth / 2) * pixelSize) + camera->up() * ((y - windowHeight / 2) * pixelSize);
            Vector ray = imagePlanePoint - camera->position();

            // printf("Image plane point: %s\n", imagePlanePoint.toString());
            //printf("Ray: %s\n", ray.toString());

            // printf("%s\n", camera->right().scalar((x - windowWidth / 2) * pixelSize).toString());

            // y = windowHeight + 1;
            // break;

            //printf("%d / %d --- ", y * windowWidth + x, windowWidth * windowHeight);
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

        for (int i = 0; i < 1; i++) {
            if (objects[i]->intersect(prev, curr)) {
                Color c = objects[i]->color();

                c.r = c.r * (1 - h / MAX_ITER);
                c.g = c.g * (1 - h / MAX_ITER);
                c.b = c.b * (1 - h / MAX_ITER);

                //printf("%f ... FOUND\n", h);

                return c;
            }
        }
    }

    //printf(":(\n");

    return BACKGROUND_COLOR;
}