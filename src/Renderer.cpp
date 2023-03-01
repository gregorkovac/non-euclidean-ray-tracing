#include "../include/Renderer.h"

Renderer::Renderer(int windowWidth, int windowHeight) {
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    this->camera = new Camera(Vector(0, 0, 0), Vector(0, 0, 0), Vector(1, 1, 1), 70);

    this->objects = new Object*[1];

    this->objects[0] = new Sphere(1, Vector(5, 0, 0), Vector(0, 0, 0), Vector(1, 1, 1));
}

void Renderer::render(unsigned char* dataBuffer) {
    // Just for testing
    // for (int y = 0; y < windowWidth; y++) {
    //     for (int x = 0; x < windowHeight; x++)
    //     {
    //         dataBuffer[(y * windowHeight + x) * 3 + 0] = 0;
    //         dataBuffer[(y * windowHeight + x) * 3 + 1] = x;
    //         dataBuffer[(y * windowHeight + x) * 3 + 2] = y;
    //     }
    // }

    // TODO: Implement raytracing here
}