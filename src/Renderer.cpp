#include "../include/Renderer.h"

Renderer::Renderer(int frameWidth, int frameHeight)
{
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;

    // this->objects = new Object *[3];
    // this->lights = new Sphere *[1];

    // Color red = {255, 0, 0};
    // Color blue = {100, 255, 255};
    // Color white = {255, 255, 255};
    // Color green = {0, 255, 0};

    // this->camera = new Camera(Vector(0, 0, 0), Vector(0, 0, 0), Vector(1, 1, 1));

    // this->objects[0] = new Sphere(1, Vector(0, 0, 3), Vector(0, 0, 0), Vector(1, 1, 1), red, 0, 0);
    // this->objects[1] = new Sphere(1, Vector(0.5, 0, 2), Vector(0, 0, 0), Vector(0.2, 0.2, 0.2), blue, 0, 0);
    // this->objects[2] = new Plane(Vector(0, 1, 0), Vector(0, -1, 0), Vector(0, 0, 0), Vector(1, 1, 1), white);
    // this->objects[3] = new Plane(Vector(0, 0, 4), Vector(0, 0, -1), Vector(0, 0, 0), Vector(1, 1, 1), green);

    // this->lights[0] = new Sphere(1, Vector(0, 0, 2), Vector(0, 0, 0), Vector(1, 1, 1), white);
}

void Renderer::parseScene(char *sceneFilePath)
{
    FILE *file = fopen(sceneFilePath, "r");

    if (file == NULL)
    {
        printf("\033[0;31m");
        printf("\nERROR: Could not open scene file %s\n", sceneFilePath);
        exit(1);
    }

    char line[256];

    this->numObjects = 0;
    this->numLights = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (&line[0] == "#")
            continue;

        char objectType[20];
        sscanf(line, "%s", objectType);

        if (strcmp(objectType, "Light") == 0)
            this->numLights++;
        else if (strcmp(objectType, "Camera") != 0)
            this->numObjects++;
    }

    this->objects = new Object *[numObjects];
    this->lights = new Sphere *[numLights];

    rewind(file);

    int objectIndex = 0, lightIndex = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (&line[0] == "#")
            continue;

        char objectType[20];
        Vector position;
        Vector rotation;
        Vector scale;
        Color color;
        float reflectivity;
        float translucency;

        sscanf(line, "%s", objectType);

        if (strcmp(objectType, "Light") == 0)
        {
            sscanf(line, "%s (%f %f %f) (%f %f %f) (%f %f %f) %f %f", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z, &color.r, &color.g, &color.b);
            this->lights[lightIndex] = new Sphere(1, position, rotation, scale, color);
            lightIndex++;
            printf(" -> Light\n");
        }
        else if (strcmp(objectType, "Camera") == 0)
        {
            sscanf(line, "%s (%f %f %f) (%f %f %f)", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z);
            this->camera = new Camera(position, rotation, Vector(1, 1, 1));
            printf(" -> Camera\n");
        }
        else if (strcmp(objectType, "Sphere") == 0)
        {

            sscanf(line, "%s (%f %f %f) (%f %f %f) (%f %f %f) (%d %d %d) %f %f", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z, &color.r, &color.g, &color.b, &reflectivity, &translucency);
            this->objects[objectIndex] = new Sphere(1, position, rotation, scale, color, reflectivity, translucency);
            objectIndex++;
            printf(" -> Sphere\n");
        }
        else if (strcmp(objectType, "Plane") == 0)
        {
            Vector normal;
            sscanf(line, "%s (%f %f %f) (%f %f %f) (%f %f %f)(%f %f %f) (%f %f %f) %f %f %f", objectType, &normal.x, &normal.y, &normal.z, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z, &color.r, &color.g, &color.b, &reflectivity, &translucency);
            this->objects[objectIndex] = new Plane(normal, position, rotation, scale, color, reflectivity, translucency);
            objectIndex++;
            printf(" -> Plane\n");
        }
        else
        {
            printf(" -> Unknown object type: %s; skipping\n", objectType);
        }
    }

    fclose(file);
}

void Renderer::render(unsigned char *dataBuffer)
{
    unsigned char data[IMAGE_PLANE_WIDTH * IMAGE_PLANE_HEIGHT * 3];

    Vector imagePlaneCenter = camera->position() + camera->forward() * FOCAL_LENGTH;

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

            int numRendered = y * IMAGE_PLANE_WIDTH + x + 1;
            int numTotal = IMAGE_PLANE_WIDTH * IMAGE_PLANE_HEIGHT;
            int numPercent = (numRendered * 100) / numTotal;

            printf("\r-> Rendered pixel %d/%d (%d%%) ", numRendered, numTotal, numPercent);

            for (int i = 0; i < numPercent / 2; i++)
                printf("■");

            for (int i = 0; i < 50 - numPercent / 2; i++)
                printf("-");

            fflush(stdout);
        }
    }
    printf("\n");

    if (DRAW_GIZMOS)
    {
        printf("Drawing gizmos...\n");

        for (int i = 0; i < numLights; i++)
        {
            float xf = (float)lights[i]->position().x;
            float yf = (float)lights[i]->position().y;

            // Map x and y from world space to image space
            Vector image_plane_center = camera->position() + camera->forward() * FOCAL_LENGTH;

            int x = (int)((xf - image_plane_center.x) / PIXEL_SIZE + IMAGE_PLANE_WIDTH / 2);
            int y = (int)((yf - image_plane_center.y) / PIXEL_SIZE + IMAGE_PLANE_HEIGHT / 2);

            if (x < 0 || x >= IMAGE_PLANE_WIDTH || y < 0 || y >= IMAGE_PLANE_HEIGHT)
                continue;

            // Draw a circle around the light
            for (int j = 0; j < 360; j++)
            {
                for (int k = 0; k < 7; k++)
                {
                    int xCircle = (int)(x + cos(j) * k);
                    int yCircle = (int)(y + sin(j) * k);

                    data[(yCircle * IMAGE_PLANE_WIDTH + xCircle) * 3] = 240;
                    data[(yCircle * IMAGE_PLANE_WIDTH + xCircle) * 3 + 1] = 200;
                    data[(yCircle * IMAGE_PLANE_WIDTH + xCircle) * 3 + 2] = 50;
                }
            }

            for (int j = -5; j < 3; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    data[((y + j) * IMAGE_PLANE_WIDTH + x - 3 + k) * 3] = 220;
                    data[((y + j) * IMAGE_PLANE_WIDTH + x - 3 + k) * 3 + 1] = 140;
                    data[((y + j) * IMAGE_PLANE_WIDTH + x - 3 + k) * 3 + 2] = 20;
                }
            }

            for (int j = -3; j < 3; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    data[((y - 5 + k) * IMAGE_PLANE_WIDTH + x + j) * 3] = 220;
                    data[((y - 5 + k) * IMAGE_PLANE_WIDTH + x + j) * 3 + 1] = 140;
                    data[((y - 5 + k) * IMAGE_PLANE_WIDTH + x + j) * 3 + 2] = 20;
                }
            }
        }
    }

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

Color Renderer::trace(Vector ray, Vector origin, int depth)
{
    if (depth > MAX_DEPTH)
        return SKY_COLOR;

    Vector curr = origin;
    Vector prev = origin;

    ray = ray.normalize3();

    for (float h = 0; h < MAX_ITER; h += 1)
    {
        prev = curr;
        curr = origin + ray * (h * STEP_SIZE);

        for (int i = 0; i < this->numObjects; i++)
        {
            if (objects[i]->intersect(prev, curr))
            {
                Color c = objects[i]->color();
                Vector intersection = objects[i]->newtonsMethod((prev + curr) / 2);

                Color reflectionColor = {0,0,0}, refractionColor = {0,0,0};

                float reflectivity = objects[i]->reflectivity();
                float translucency = objects[i]->translucency();

                if (reflectivity > 0)
                {
                    Vector reflectionRay = ray - objects[i]->normal(intersection) * (objects[i]->normal(intersection) * 2 * ray);

                    reflectionColor = trace(reflectionRay, intersection + reflectionRay, depth + 1);

                    // c.r = c.r * (1 - reflectivity) + c1.r * reflectivity;
                    // c.g = c.g * (1 - reflectivity) + c1.g * reflectivity;
                    // c.b = c.b * (1 - reflectivity) + c1.b * reflectivity;
                }

                if (translucency > 0)
                {
                    Vector refractionRay = ray - objects[i]->normal(intersection) * (objects[i]->normal(intersection) * 2 * ray);

                    refractionColor = trace(refractionRay, intersection + refractionRay, depth + 1);

                    // c.r = c.r * (1 - translucency) + c1.r * translucency;
                    // c.g = c.g * (1 - translucency) + c1.g * translucency;
                    // c.b = c.b * (1 - translucency) + c1.b * translucency;
                }

                for (int j = 0; j < this->numLights; j++)
                {
                    if (isShadowed(intersection, lights[j]->position()))
                    {
                        c.r = c.r * 0.2;
                        c.g = c.g * 0.2;
                        c.b = c.b * 0.2;
                    }
                    else
                    {
                        float distToLight = intersection.normalize3().distance(lights[j]->position());

                        distToLight *= distToLight;

                        c.r /= distToLight;
                        c.g /= distToLight;
                        c.b /= distToLight;
                    }
                }

                c.r = c.r + reflectionColor.r * reflectivity + refractionColor.r * translucency;
                c.g = c.g + reflectionColor.g * reflectivity + refractionColor.g * translucency;
                c.b = c.b + reflectionColor.b * reflectivity + refractionColor.b * translucency;

                return c;
            }
        }
    }

    return SKY_COLOR;
}

bool Renderer::isShadowed(Vector origin, Vector light)
{

    Vector ray = (light - origin).normalize3();

    Vector originMoved = origin + ray;

    Vector curr = originMoved;
    Vector prev = originMoved;

    for (float h = 0; h < MAX_ITER; h += 1)
    {
        prev = curr;
        curr = originMoved + ray * (h * STEP_SIZE);

        for (int i = 0; i < this->numObjects; i++)
            if (objects[i]->intersect(prev, curr) && objects[i]->translucency() == 0)
                return true;
    }

    return false;
}