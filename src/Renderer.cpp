#include "../include/Renderer.h"

Renderer::Renderer(int windowWidth, int windowHeight, float pixelSize) {
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->pixelSize = pixelSize;

    this->camera = new Camera(Vector(0, 0, 0), Vector(0, 0, 0), Vector(1, 1, 1), 70);

    this->objects = new Object*[1];

    this->objects[0] = new Sphere(1, Vector(5, 0, 0), Vector(0, 0, 0), Vector(1, 1, 1));
}

void Renderer::render(unsigned char* dataBuffer) {
    Vector imagePlaneCenter = camera->position().add(camera->forward().scalar(camera->f()));

    for (int y = 0; y < windowHeight; y++) {
        for (int x = 0; x < windowWidth; x++) {
            Vector imagePlanePoint = imagePlaneCenter.add(camera->right().scalar((x - windowWidth / 2) * pixelSize)).add(camera->up().scalar((y - windowHeight / 2) * pixelSize));
            Vector ray = imagePlanePoint.sub(camera->position()).normalize();
        }
    }
    
}