#include "../include/Renderer.h"

Renderer::Renderer(int frameWidth, int frameHeight)
{
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
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

    bool cameraFound = false;

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '#' || line[0] == '\n')
            continue;

        char objectType[20];
        sscanf(line, "%s", objectType);

        if (strcmp(objectType, "Light") == 0)
            this->numLights++;
        else if (strcmp(objectType, "Camera") != 0)
            this->numObjects++;
    }

    this->objects = new Object *[numObjects];
    this->lights = new Light *[numLights];

    rewind(file);

    int objectIndex = 0, lightIndex = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '#' || line[0] == '\n')
            continue;

        char objectType[20];
        Vector position;
        Vector rotation;
        Vector scale;
        Color color;
        char colorType[20];
        float translucency;
        float refractiveIndex;
        float reflectivity;
        float intensity;
        float innerRadius, outerRadius;

        sscanf(line, "%s", objectType);

        if (strcmp(objectType, "Light") == 0)
        {
            sscanf(line, "%s (%f %f %f) (%d %d %d) %f", objectType, &position.x, &position.y, &position.z, &color.r, &color.g, &color.b, &intensity);
            this->lights[lightIndex] = new Light(position, color, intensity);
            lightIndex++;
            printf(" -> Light\n");
        }
        else if (strcmp(objectType, "Camera") == 0)
        {
            cameraFound = true;
            sscanf(line, "%s (%f %f %f) (%f %f %f)", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z);
            this->camera = new Camera(position, rotation, Vector(1, 1, 1));
            printf(" -> Camera\n");
        }
        else if (strcmp(objectType, "Sphere") == 0)
        {
            sscanf(line, "%s (%f %f %f) (%f %f %f) (%f %f %f) %f %f %f %s (%d %d %d)", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z, &reflectivity, &translucency, &refractiveIndex, colorType, &color.r, &color.g, &color.b);

            if (scale.x != scale.y || scale.x != scale.z || scale.y != scale.z)
            {
                printf("\033[0;31m");
                printf("\nERROR: Sphere scale must be equal in all dimensions\n");
                printf("\033[0m");
                exit(1);
            }

            // printf("%s %d %d %d %f %f\n", colorType, color.r, color.g, color.b, reflectivity, translucency);

            this->objects[objectIndex] = new Sphere(1, position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType);
            objectIndex++;
            printf(" -> Sphere\n");
        }
        else if (strcmp(objectType, "Plane") == 0)
        {
            sscanf(line, "%s (%f %f %f) (%f %f %f) (%f %f %f) %f %f %f %s (%d %d %d)", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z, &reflectivity, &translucency, &refractiveIndex, colorType, &color.r, &color.g, &color.b);
            this->objects[objectIndex] = new Plane(position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType);

            objectIndex++;
            printf(" -> Plane\n");
        }
        else if (strcmp(objectType, "Torus") == 0)
        {
            sscanf(line, "%s (%f %f %f) (%f %f %f) (%f %f %f) %f %f %f %f %f %s (%d %d %d)", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z, &innerRadius, &outerRadius, &reflectivity, &translucency, &refractiveIndex, colorType, &color.r, &color.g, &color.b);
            this->objects[objectIndex] = new Torus(outerRadius, innerRadius, position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType);

            objectIndex++;
            printf(" -> Torus\n");
        }

        else
        {
            printf(" -> Unknown object type: %s; skipping\n", objectType);
        }
    }

    fclose(file);

    if (!cameraFound)
    {
        printf("\033[0;31m");
        printf("\nERROR: No camera found in scene file %s\n", sceneFilePath);
        printf("\033[0m");
        exit(1);
    }

    if (this->numLights == 0 && !USE_DIRECTIONAL_LIGHT)
    {
        printf("\033[0;31m");
        printf("\nERROR: Directional light is disabled and no lights found in scene file %s\n", sceneFilePath);
        printf("\033[0m");
        exit(1);
    }
}

// TODO: Multiple shadow rays under different angles

void Renderer::render(unsigned char *dataBuffer)
{
    if (PRINT_OBJECTS_ON_STARTUP)
    {
        printf("\nObjects:\n");
        for (int i = 0; i < numObjects; i++)
        {
            printf(" -> %s\n\n\n", objects[i]->toString());
        }
    }

    unsigned char data[IMAGE_PLANE_WIDTH * IMAGE_PLANE_HEIGHT * 3];

    Vector imagePlaneCenter = camera->position() + camera->forward() * FOCAL_LENGTH;

    for (int y = 0; y < IMAGE_PLANE_HEIGHT; y++)
    {
        for (int x = 0; x < IMAGE_PLANE_WIDTH; x++)
        {
            Color color = {0, 0, 0};
            for (int k = 0; k < RAYS_PER_PIXEL; k++)
            {
                Vector offset = Vector(randomBetween(-PIXEL_SIZE / 2, PIXEL_SIZE / 2), randomBetween(-PIXEL_SIZE / 2, PIXEL_SIZE / 2), 0);

                Vector imagePlanePoint = imagePlaneCenter + camera->right() * ((x - IMAGE_PLANE_WIDTH / 2) * PIXEL_SIZE) + camera->up() * ((y - IMAGE_PLANE_HEIGHT / 2) * PIXEL_SIZE);

                imagePlanePoint = imagePlanePoint + offset;

                Vector ray = PROJECTION == 0 ? (imagePlanePoint - camera->position()) : (camera->forward());

                Color c = trace(ray, imagePlaneCenter, 0);
                color.r += c.r;
                color.g += c.g;
                color.b += c.b;
            }

            color.r /= RAYS_PER_PIXEL;
            color.g /= RAYS_PER_PIXEL;
            color.b /= RAYS_PER_PIXEL;

            data[(y * IMAGE_PLANE_WIDTH + x) * 3] = color.r * BRIGHTNESS > 255 ? 255 : color.r * BRIGHTNESS;
            data[(y * IMAGE_PLANE_WIDTH + x) * 3 + 1] = color.g * BRIGHTNESS > 255 ? 255 : color.g * BRIGHTNESS;
            data[(y * IMAGE_PLANE_WIDTH + x) * 3 + 2] = color.b * BRIGHTNESS > 255 ? 255 : color.b * BRIGHTNESS;

            int numRendered = y * IMAGE_PLANE_WIDTH + x + 1;
            int numTotal = IMAGE_PLANE_WIDTH * IMAGE_PLANE_HEIGHT;
            int numPercent = (numRendered * 100) / numTotal;

            printf("\r -> Rendered pixel %d/%d (%d%%) ", numRendered, numTotal, numPercent);

            for (int i = 0; i < numPercent / 2; i++)
            {
                printf("■");
            }

            for (int i = 0; i < 50 - numPercent / 2; i++)
            {
                printf("-");
            }

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

    // ray = ray.normalize3();

    Color lightIntersectColor = {0, 0, 0};
    float lightIntersectDist = -1;

    for (float h = 1; h < MAX_ITER; h += 1)
    {
        prev = curr;
        curr = origin + ray * h * STEP_SIZE;

        // printf("%s %s\n", prev.toString(), curr.toString());

        for (int i = 0; i < this->numLights; i++)
        {
            float lDist = curr.distance(lights[i]->position());
            if (lDist < 0.1 && (lDist < lightIntersectDist || lightIntersectDist == -1))
            {
                // TODO: Namesto return prištej to končni barvi in upoštevaj oddaljenost
                lightIntersectColor = lights[i]->color();
                lightIntersectDist = lDist;
            }
        }

        for (int i = 0; i < this->numObjects; i++)
        {
            // if (ray == Vector(0.99, -1, 1)) {
            //     printf("%s %s\n", prev.toString(), curr.toString());
            //     printf("%f %f\n\n", objects[i]->equation(prev), objects[i]->equation(curr));
            // }

            Vector intersection;

            if (objects[i]->isEquationParametric())
            {
                // TODO: Parametric equation object class
                intersection = ((Torus *)objects[i])->solveParametric(origin, ray, h * STEP_SIZE);

                printf("%s\n", intersection.toString());

                if (intersection.w == -1)
                    continue;
            }
            else if (objects[i]->intersect(prev, curr))
            {
                intersection = objects[i]->gaussNewtonsMethod((prev + curr) / 2);
            }
            else
            {
                continue;
            }

            // Color c = objects[i]->color(intersection);
            //  Color c = {0,0,0};
            if (!objects[i]->shade())
                return objects[i]->color((prev + curr) / 2);

            Color c = {
                AMBIENT_LIGHT_COLOR.r * AMBIENT_LIGHT_INTENSITY,
                AMBIENT_LIGHT_COLOR.g * AMBIENT_LIGHT_INTENSITY,
                AMBIENT_LIGHT_COLOR.b * AMBIENT_LIGHT_INTENSITY};
            Color objectColor = objects[i]->color(intersection);
            Color reflectionColor = {0, 0, 0}, refractionColor = {0, 0, 0};

            float reflectivity = objects[i]->reflectivity();
            float translucency = objects[i]->translucency();

            Vector inRay = (intersection - origin).normalize3();
            Vector normal = objects[i]->normal(intersection);

            // TODO: Phong - specular and diffuse

            if (reflectivity > 0)
            {
                Vector reflectionRay = inRay - 2 * normal * (normal * inRay);

                reflectionColor = trace(reflectionRay, intersection + reflectionRay * STEP_SIZE, depth + 1);

                c.r += reflectivity * reflectionColor.r;
                c.g += reflectivity * reflectionColor.g;
                c.b += reflectivity * reflectionColor.b;
            }

            if (translucency > 0)
            {

                Vector refractionRay = objects[i]->refractiveCoefficient(curr) * (inRay - normal * (normal * inRay)) - normal * sqrt(1 - pow(objects[i]->refractiveCoefficient(curr), 2) * (1 - pow(normal * inRay, 2)));

                // float inAngle = acos(-(inRay * normal));
                // float outAngle = asin(objects[i]->refractiveCoefficient(curr) * sin(inAngle));
                // Matrix rotation = Matrix::rotation(Vector(outAngle - inAngle, outAngle - inAngle, outAngle - inAngle));
                // Vector refractionRay = rotation * inRay;

                // printf("it=%d obj_i=%d coef=%f vec=%s\n", depth, i, objects[i]->refractiveCoefficient(curr), refractionRay.toString());

                // printf("I:%s\nR:%s\n\n", inRay.toString(), refractionRay.toString());

                // refractionRay = refractionRay.normalize3();
                refractionColor = trace(refractionRay, intersection + refractionRay * STEP_SIZE, depth + 1);

                c.r += translucency * refractionColor.r;
                c.g += translucency * refractionColor.g;
                c.b += translucency * refractionColor.b;
            }

            // float colorFactor = reflectivity == 0 && translucency == 0 ? 1 : min(1.0, reflectivity + translucency);

            /* PHONG:
            c = c_a * k_a + sum ( c_i * (k_d * (l_i * n) + k_s * (R_i * e) ^ p ))
            c_i ... jakost luči
            l_i ... vektor proti luči
            k_d ... diffuse factor
            k_s ... specular factor
            R_i ... idealni odboj, R = 2 * (l * n) * n - l
            e ... vektor proti kameri
            p ... parameter zrcalnega odboja
            */

            // c.r = AMBIENT_LIGHT_INTENSITY * AMBIENT_LIGHT_COLOR.r + (1 - reflectivity) * (1 - translucency) * c.r + reflectionColor.r * reflectivity + refractionColor.r * translucency;
            // c.g = AMBIENT_LIGHT_INTENSITY * AMBIENT_LIGHT_COLOR.g + (1 - reflectivity) * (1 - translucency) * c.g + reflectionColor.g * reflectivity + refractionColor.g * translucency;
            // c.b = AMBIENT_LIGHT_INTENSITY * AMBIENT_LIGHT_COLOR.b + (1 - reflectivity) * (1 - translucency) * c.b + reflectionColor.b * reflectivity + refractionColor.b * translucency;

            for (int j = 0; j < this->numLights; j++)
            {
                if (!isShadowed(intersection, lights[j]->position()))
                {
                    // float distToLight = intersection.normalize3().distance(lights[j]->position());
                    float distToLight = intersection.distance(lights[j]->position());

                    distToLight *= distToLight;

                    // float k_d = 0.5;
                    // float k_s = 0.5;
                    // Vector l = (lights[j]->position() - intersection).normalize3();
                    // Vector R = normal * 2 * (l * normal) - l;
                    // Vector e = (origin - intersection).normalize3();
                    // float p = 1;

                    // c.r += lights[j]->color(intersection).r * (objects[i]->color(intersection).r * k_d * (l * normal) + objects[i]->color(intersection).r * k_s * pow((R * e), p));
                    // c.g += lights[j]->color(intersection).g * (objects[i]->color(intersection).g * k_d * (l * normal) + objects[i]->color(intersection).g * k_s * pow((R * e), p));
                    // c.b += lights[j]->color(intersection).b * (objects[i]->color(intersection).b * k_d * (l * normal) + objects[i]->color(intersection).b * k_s * pow((R * e), p));

                    // TODO: light color

                    // objectColor.r /= distToLight;
                    // objectColor.g /= distToLight;
                    // objectColor.b /= distToLight;

                    c.r += lights[j]->intensity() * lights[j]->color().r / 255.0 * objectColor.r / distToLight;
                    c.g += lights[j]->intensity() * lights[j]->color().g / 255.0 * objectColor.g / distToLight;
                    c.b += lights[j]->intensity() * lights[j]->color().b / 255.0 * objectColor.b / distToLight;

                    // printf("%d %d %d\n", lights[j]->color().r, lights[j]->color().g, lights[j]->color().b);

                    // c.r += 0.05 * lights[j]->color(intersection).r;
                    // c.g += 0.05 * lights[j]->color(intersection).g;
                    // c.b += 0.05 * lights[j]->color(intersection).b;
                }
                else
                {
                    // objectColor.r *= 0.05;
                    // objectColor.g *= 0.05;
                    // objectColor.b *= 0.05;
                }
            }

            // Directional light
            Vector directionalLight = 100 * Vector(-DIRECTIONAL_LIGHT_DIRECTION_X, -DIRECTIONAL_LIGHT_DIRECTION_Y, -DIRECTIONAL_LIGHT_DIRECTION_Z);

            if (!isShadowed(intersection, directionalLight))
            {
                c.r += DIRECTIONAL_LIGHT_INTENSITY * DIRECTIONAL_LIGHT_COLOR.r / 255.0 * objectColor.r;
                c.g += DIRECTIONAL_LIGHT_INTENSITY * DIRECTIONAL_LIGHT_COLOR.g / 255.0 * objectColor.g;
                c.b += DIRECTIONAL_LIGHT_INTENSITY * DIRECTIONAL_LIGHT_COLOR.b / 255.0 * objectColor.b;
            }

            // if (depth > 1) {
            //     printf("%d (%d %d %d)\n", i, c.r, c.g, c.b);
            // }

            // c.r += objectColor.r;
            // c.g += objectColor.g;
            // c.b += objectColor.b;

            // if (lightIntersectDist != -1)
            // {
            //     lightIntersectDist = (1 - 10 * lightIntersectDist);

            //     c.r = (int) ((float)c.r * lightIntersectDist * (float)lightIntersectColor.r);
            //     c.g = (int) ((float)c.g * lightIntersectDist * (float)lightIntersectColor.g);
            //     c.b = (int) ((float)c.b * lightIntersectDist * (float)lightIntersectColor.b);
            // }

            return c;
        }
    }

    // if (lightIntersectDist != -1)
    // {
    //     Color sky = SKY_COLOR;

    //     lightIntersectDist = (1 - 10 * lightIntersectDist);

    //     sky.r = (int) ((float)sky.r * lightIntersectDist * (float)lightIntersectColor.r);
    //     sky.g = (int) ((float)sky.g * lightIntersectDist * (float)lightIntersectColor.g);
    //     sky.b = (int) ((float)sky.b * lightIntersectDist * (float)lightIntersectColor.b);

    //     return sky;
    // }

    return SKY_COLOR;
}

bool Renderer::isShadowed(Vector origin, Vector light)
{
    Vector ray = (light - origin).normalize3();

    Vector originMoved = origin + ray * STEP_SIZE;

    Vector curr = originMoved;
    Vector prev = originMoved;

    for (float h = 1; h < MAX_ITER; h += 1)
    {
        prev = curr;
        curr = originMoved + ray * (h * STEP_SIZE);

        if (curr.distance(light) < EPSILON)
            return false;

        for (int i = 0; i < this->numObjects; i++)
            if (objects[i]->intersect(prev, curr) && objects[i]->translucency() == 0)
            {
                // printf("%s\n\n", objects[i]->toString());
                return true;
            }
    }

    return false;
}