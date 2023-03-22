#include "../include/Renderer.h"

#include <stdio.h>

Renderer::Renderer(int frameWidth, int frameHeight)
{
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;

    this->camera = new Camera(Vector(0, 0, 0), Vector(0, 0, 0), Vector(1, 1, 1), 1);

    this->objects = new Object *[3];
    this->lights = new Sphere *[1];

    Color red = {255, 0, 0};
    Color blue = {100, 255, 255};
    Color white = {255, 255, 255};

    this->objects[0] = new Sphere(1, Vector(-0.5, 0, 3), Vector(0, 0, 0), Vector(1, 1, 1), red, 0, 0);
    this->objects[1] = new Sphere(1, Vector(-1, 0, 1.5), Vector(0, 0, 0), Vector(0.5, 0.5, 0.5), blue, 0, 0);
    this->objects[2] = new Plane(Vector(0, 1, 0), Vector(0, -1, 0), Vector(0, 0, 0), Vector(1, 1, 1), white);

    this->lights[0] = new Sphere(1, Vector(-1, 1, 0), Vector(0, 0, 0), Vector(1, 1, 1), white);
}

// TODO: Separate window size and image plane size
// TODO: More accurate shading. Problem with Newton's method?

void Renderer::render(unsigned char *dataBuffer)
{
    unsigned char data[IMAGE_PLANE_WIDTH * IMAGE_PLANE_HEIGHT * 3];

    Vector imagePlaneCenter = camera->position() + camera->forward() * camera->f();

    for (int y = 0; y < IMAGE_PLANE_HEIGHT; y++)
    {
        for (int x = 0; x < IMAGE_PLANE_WIDTH; x++)
        {
            Vector imagePlanePoint = imagePlaneCenter + camera->right() * ((x - IMAGE_PLANE_WIDTH / 2) * PIXEL_SIZE) + camera->up() * ((y - IMAGE_PLANE_HEIGHT / 2) * PIXEL_SIZE);
            Vector ray = imagePlanePoint - camera->position();

            Color color = trace(ray, camera->position(), 0);

            data[(y * IMAGE_PLANE_WIDTH + x) * 3] = color.r * BRIGHTNESS > 255 ? 255 : color.r * BRIGHTNESS;
            data[(y * IMAGE_PLANE_WIDTH + x) * 3 + 1] = color.g * BRIGHTNESS > 255 ? 255 : color.g * BRIGHTNESS;
            data[(y * IMAGE_PLANE_WIDTH + x) * 3 + 2] = color.b * BRIGHTNESS > 255 ? 255 : color.b * BRIGHTNESS;

            // Loading bar for number of rendered pixels
            int numRendered = y * IMAGE_PLANE_WIDTH + x + 1;
            int numTotal = IMAGE_PLANE_WIDTH * IMAGE_PLANE_HEIGHT;
            int numPercent = (numRendered * 100) / numTotal;
            // Loading bar printing
            printf("\r-> Rendered pixel %d/%d (%d%%)", numRendered, numTotal, numPercent);

            //printf("\r-> Rendered pixel %d/%d", y * frameWidth + x + 1, frameWidth * frameHeight);
            fflush(stdout);
        }
    }
    printf("\n");

    printf("Mapping image plane to window...\n");

    // Map data to dataBuffer
    for (int y = 0; y < frameHeight; y++)
    {
        for (int x = 0; x < frameWidth; x++)
        {
            int xPlane = x * IMAGE_PLANE_WIDTH / frameWidth;
            int yPlane = y * IMAGE_PLANE_HEIGHT / frameHeight;

            dataBuffer[(y * frameWidth + x) * 3] = data[(yPlane * IMAGE_PLANE_WIDTH + xPlane) * 3];
            dataBuffer[(y * frameWidth + x) * 3 + 1] = data[(yPlane * IMAGE_PLANE_WIDTH + xPlane) * 3 + 1];
            dataBuffer[(y * frameWidth + x) * 3 + 2] = data[(yPlane * IMAGE_PLANE_WIDTH + xPlane) * 3 + 2];
        }
    }
}

// TODO: Better lighting
// TODO: Directional light

Color Renderer::trace(Vector ray, Vector origin, int depth)
{
    if (depth > MAX_DEPTH)
        return SKY_COLOR;

    Vector curr = origin;
    Vector prev = origin;

    for (float h = 0; h < MAX_ITER; h += 1)
    {
        prev = curr;
        curr = origin + ray * (h * STEP_SIZE);

        // TODO: Define scene in seperate file and then parse it. Take into acount the number of objects and use it here.
        for (int i = 0; i < 3; i++)
        {
            if (objects[i]->intersect(prev, curr))
            {
                Color c = objects[i]->color();
                Vector intersection = objects[i]->newtonsMethod((prev + curr) / 2);

                float reflectivity = objects[i]->reflectivity();
                float translucency = objects[i]->translucency();

                if (reflectivity > 0)
                {
                    Vector reflectionRay = ray - objects[i]->normal(intersection) * (objects[i]->normal(intersection) * 2 * ray);

                    Color c1 = trace(reflectionRay, intersection + reflectionRay, depth + 1);

                    c.r = c.r * (1 - reflectivity) + c1.r * reflectivity;
                    c.g = c.g * (1 - reflectivity) + c1.g * reflectivity;
                    c.b = c.b * (1 - reflectivity) + c1.b * reflectivity;
                }

                if (translucency > 0)
                {
                    Vector refractionRay = ray - objects[i]->normal(intersection) * (objects[i]->normal(intersection) * 2 * ray);

                    Color c1 = trace(refractionRay, intersection + refractionRay, depth + 1);

                    c.r = c.r * (1 - translucency) + c1.r * translucency;
                    c.g = c.g * (1 - translucency) + c1.g * translucency;
                    c.b = c.b * (1 - translucency) + c1.b * translucency;
                }
                
                for (int j = 0; j < 1; j++)
                {
                    if (isShadowed(intersection, lights[j]->position()))
                    {
                        c.r = c.r * 0.1;
                        c.g = c.g * 0.1;
                        c.b = c.b * 0.1;
                    }
                    else
                    {
                        float distToLight = intersection.normalize().distance(lights[j]->position());

                        distToLight *= distToLight;

                        c.r /= distToLight;
                        c.g /= distToLight;
                        c.b /= distToLight;
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

    Vector ray = (light - origin).normalize();

    Vector originMoved = origin + ray;

    Vector curr = originMoved;
    Vector prev = originMoved;

    for (float h = 0; h < MAX_ITER; h += 1)
    {
        prev = curr;
        curr = originMoved + ray * (h * STEP_SIZE);

        for (int i = 0; i < 3; i++)
            if (objects[i]->intersect(prev, curr) && objects[i]->translucency() == 0)
                return true;
    }

    return false;
}