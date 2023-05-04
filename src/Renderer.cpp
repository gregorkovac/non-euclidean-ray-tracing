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
        char colorType[1000];
        float translucency;
        float refractiveIndex;
        float reflectivity;
        float intensity;
        float paramA, paramB, paramC;
        char normalMap[1000];

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
            sscanf(line, "%s (%f %f %f) (%f %f %f) (%f %f %f) %f %f %f %s %s (%d %d %d)", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z, &reflectivity, &translucency, &refractiveIndex, colorType, normalMap, &color.r, &color.g, &color.b);

            if (scale.x != scale.y || scale.x != scale.z || scale.y != scale.z)
            {
                printf("\033[0;31m");
                printf("\nERROR: Sphere scale must be equal in all dimensions\n");
                printf("\033[0m");
                exit(1);
            }

            this->objects[objectIndex] = new Sphere(1, position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType, normalMap);
            objectIndex++;
            printf(" -> Sphere\n");
        }
        else if (strcmp(objectType, "Plane") == 0)
        {
            sscanf(line, "%s (%f %f %f) (%f %f %f) (%f %f %f) %f %f %f %s %s (%d %d %d)", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z, &reflectivity, &translucency, &refractiveIndex, colorType, normalMap, &color.r, &color.g, &color.b);
            this->objects[objectIndex] = new Plane(position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType, normalMap);

            objectIndex++;
            printf(" -> Plane\n");
        }
        else if (strcmp(objectType, "Torus") == 0)
        {
            sscanf(line, "%s (%f %f %f) (%f %f %f) (%f %f %f) %f %f %f %f %f %s %s (%d %d %d)", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z, &paramA, &paramB, &reflectivity, &translucency, &refractiveIndex, colorType, normalMap, &color.r, &color.g, &color.b);
            this->objects[objectIndex] = new Torus(paramB, paramA, position, rotation, scale, color, reflectivity, translucency, refractiveIndex, normalMap, colorType);

            objectIndex++;
            printf(" -> Torus\n");
        }
        else if (strcmp(objectType, "Hyperboloid") == 0)
        {
            sscanf(line, "%s (%f %f %f) (%f %f %f) (%f %f %f) %f %f %f %f %f %f %s %s (%d %d %d)", objectType, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z, &paramA, &paramB, &paramC, &reflectivity, &translucency, &refractiveIndex, colorType, normalMap, &color.r, &color.g, &color.b);
            this->objects[objectIndex] = new Hyperboloid(paramA, paramB, paramC, position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType, normalMap);

            objectIndex++;
            printf(" -> Hyperboloid\n");
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

FILE *pointFile;

float sphereRadius;

void Renderer::render(unsigned char *dataBuffer)
{

    if (PLOT_RAYS)
        pointFile = fopen("points.txt", "w");

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
                // Vector offset = Vector(randomBetween(-PIXEL_SIZE / 2, PIXEL_SIZE / 2), randomBetween(-PIXEL_SIZE / 2, PIXEL_SIZE / 2), 0);
                Vector offset = Vector(0, 0, 0);

                Vector imagePlanePoint = imagePlaneCenter + camera->right() * ((x - IMAGE_PLANE_WIDTH / 2) * PIXEL_SIZE) + camera->up() * ((y - IMAGE_PLANE_HEIGHT / 2) * PIXEL_SIZE);

                imagePlanePoint = imagePlanePoint + offset;

                Vector ray = PROJECTION == 0 ? (imagePlanePoint - camera->position()) : (camera->forward());

                Color c = trace(ray, camera->position(), 0);
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

            auto now = std::chrono::high_resolution_clock::now();
            float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->executionStart).count() / 1000.0f;
            int seconds = (int)elapsed % 60;
            int minutes = (int)elapsed / 60;

            printf("\r [%02d:%02d] Rendered pixel %d/%d (%d%%) ", minutes, seconds, numRendered, numTotal, numPercent);

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

    if (PLOT_RAYS)
    {
        fclose(pointFile);
        system("python3 other/plot_rays.py");
    }
}

Color Renderer::trace(Vector ray, Vector origin, int depth, int maxIter, float *distanceTravelled, Color *unlitColor)
{
    if (depth > MAX_DEPTH)
        return SKY_COLOR;

    // Vector curr = origin;
    // Vector prev = origin;

    // UV uvOrigin = this->VectorToUV(origin);
    // UV uvRay = this->VectorToUV(ray);

    Vector prev, curr;
    // UV uvPrev, uvCurr;

    Vector originOffset = Vector(0, 0, 0);

    prev = origin;
    curr = origin;

    if (SPACE_TYPE == SPHERICAL)
    {
        curr = origin + ray;
    }

    if (PLOT_RAYS)
        fprintf(pointFile, "%f %f %f\n", curr.x, curr.y, curr.z);

    UV uvPrev = this->VectorToUV(origin);
    UV uvCurr = this->VectorToUV(origin);
    UV uvOrigin = this->VectorToUV(origin);
    UV uvRay = this->VectorToUV(ray);

    UV fundamentalDomainOffset = {0, 0};

    for (float h = 1; h < maxIter; h += 1)
    {

        switch (SPACE_TYPE)
        {
        case EUCLIDEAN:
            prev = curr;
            curr = origin + ray * h * STEP_SIZE;
            break;

        case FLAT_TORUS:
            prev = curr;
            curr = origin + ray * h * STEP_SIZE;

            curr.x = mapToFundamentalDomain(curr.x, FUNDAMENTAL_DOMAIN_X_MIN, FUNDAMENTAL_DOMAIN_X_MAX);
            curr.y = mapToFundamentalDomain(curr.y, FUNDAMENTAL_DOMAIN_Y_MIN, FUNDAMENTAL_DOMAIN_Y_MAX);
            curr.z = mapToFundamentalDomain(curr.z, FUNDAMENTAL_DOMAIN_Z_MIN, FUNDAMENTAL_DOMAIN_Z_MAX);
            break;

        case MIRRORED_CUBE:
        {
            // TODO:
            prev = curr;
            curr = origin + ray * h * STEP_SIZE;

            Vector newCurr = Vector(0, 0, 0);

            newCurr.x = mapToFundamentalDomain(curr.x, FUNDAMENTAL_DOMAIN_X_MIN, FUNDAMENTAL_DOMAIN_X_MAX);
            newCurr.y = mapToFundamentalDomain(curr.y, FUNDAMENTAL_DOMAIN_Y_MIN, FUNDAMENTAL_DOMAIN_Y_MAX);
            newCurr.z = mapToFundamentalDomain(curr.z, FUNDAMENTAL_DOMAIN_Z_MIN, FUNDAMENTAL_DOMAIN_Z_MAX);

            if (newCurr != curr)
            {
                newCurr = -1 * newCurr;
            }

            curr = newCurr;
        }
        break;

        case SPHERICAL:
            uvPrev = uvCurr;
            prev = curr;

            uvRay = this->rungeKutta4(this->VectorToUV(prev), uvRay, STEP_SIZE);

            Vector rayXYZ = this->UVToVector(uvRay);

            curr = prev + rayXYZ * STEP_SIZE;

            uvCurr.u = uvPrev.u + uvRay.u * STEP_SIZE;
            uvCurr.v = uvPrev.v + uvRay.v * STEP_SIZE;

            if (PLOT_RAYS)
                fprintf(pointFile, "%f %f %f\n", curr.x, curr.y, curr.z);
            break;
        }

        for (int i = 0; i < this->numObjects; i++)
        {

            if (objects[i]->intersect(prev, curr))
            {

                Vector intersection = objects[i]->newtonsMethod((prev + curr) / 2);

                if (distanceTravelled != NULL)
                    *distanceTravelled = h * STEP_SIZE;

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

                if (reflectivity > 0)
                {
                    Vector reflectionRay = inRay - 2 * normal * (normal * inRay);

                    reflectionColor = trace(reflectionRay, intersection + reflectionRay * STEP_SIZE, depth + 1);

                    c.r *= 1 - reflectivity;
                    c.g *= 1 - reflectivity;
                    c.b *= 1 - reflectivity;

                    c.r += reflectivity * reflectionColor.r;
                    c.g += reflectivity * reflectionColor.g;
                    c.b += reflectivity * reflectionColor.b;
                }

                if (translucency > 0)
                {
                    float eta = objects[i]->refractiveCoefficient(curr);

                    if (normal * inRay > 0)
                    {
                        eta = 1 / eta;
                        normal = -1 * normal;
                    }

                    Vector refractionRay = (eta * (normal * inRay) - sqrt(1 - eta * eta * (1 - pow(normal * inRay, 2)))) * normal - eta * inRay;

                    refractionColor = trace(refractionRay, intersection + refractionRay * STEP_SIZE, depth + 1);

                    c.r *= 1 - translucency;
                    c.g *= 1 - translucency;
                    c.b *= 1 - translucency;

                    c.r += translucency * refractionColor.r;
                    c.g += translucency * refractionColor.g;
                    c.b += translucency * refractionColor.b;
                }

                if (unlitColor != NULL)
                {
                    *unlitColor = c;
                }

                c.r *= (1 - RANDOM_RAY_ABSORPTION_FACTOR);
                c.g *= (1 - RANDOM_RAY_ABSORPTION_FACTOR);
                c.b *= (1 - RANDOM_RAY_ABSORPTION_FACTOR);

                for (int j = 0; j < RANDOM_RAY_COUNT; j++)
                {
                    Vector diffuse = Vector(0, 0, 0);

                    int iterCount = 0;
                    while (iterCount < 1000)
                    {
                        Vector rand = Vector(
                            randomBetween(-1, 1),
                            randomBetween(-1, 1),
                            randomBetween(-1, 1));

                        if (rand.norm() <= 1)
                        {
                            diffuse = rand.normalize3();
                            break;
                        }

                        iterCount++;
                    }

                    if (normal * diffuse < 0)
                    {
                        diffuse = -1 * diffuse;
                    }

                    float *distTravelled = new float(0);
                    Color *unlitColor = new Color;
                    unlitColor->r = 0;
                    unlitColor->g = 0;
                    unlitColor->b = 0;

                    Color diffuseColor = trace(diffuse, intersection + diffuse * STEP_SIZE, depth + 1, maxIter = 100, distanceTravelled = distTravelled);

                    if (diffuseColor == SKY_COLOR)
                        c.r += (RANDOM_RAY_ABSORPTION_FACTOR / RANDOM_RAY_COUNT) * objectColor.r;
                    c.g += (RANDOM_RAY_ABSORPTION_FACTOR / RANDOM_RAY_COUNT) * objectColor.g;
                    c.b += (RANDOM_RAY_ABSORPTION_FACTOR / RANDOM_RAY_COUNT) * objectColor.b;
                    continue;

                    // printf("%f\n", *distTravelled * 10);

                    float diffuseFactor = (*distTravelled) * (*distTravelled);
                    if (diffuseFactor < 1)
                        diffuseFactor = 1;

                    c.r += (RANDOM_RAY_ABSORPTION_FACTOR / RANDOM_RAY_COUNT) * diffuseColor.r / diffuseFactor;
                    c.g += (RANDOM_RAY_ABSORPTION_FACTOR / RANDOM_RAY_COUNT) * diffuseColor.g / diffuseFactor;
                    c.b += (RANDOM_RAY_ABSORPTION_FACTOR / RANDOM_RAY_COUNT) * diffuseColor.b / diffuseFactor;
                }


                for (int j = 0; j < this->numLights; j++)
                {
                    
                    //Color shadowColor = {0, 0, 0};
                    float shadowR = 0, shadowG = 0, shadowB = 0;
                    for (int k = 0; k < SHADOW_RAY_COUNT; k++)
                    {
                        Vector offset;
                        float weight;

                        if (k == 0) {
                            offset = Vector(0, 0, 0);
                            weight = (255 * SHADOW_RAY_COUNT) / (2 * SHADOW_RAY_COUNT);
                        } else {
                            offset = Vector(
                                randomBetween(-1, 1),
                                randomBetween(-1, 1),
                                randomBetween(-1, 1));

                            weight = 255 / (2 * SHADOW_RAY_COUNT);
                        }

                        //Vector lightPosition = lights[j]->position() + offset;
                        Vector shadowRay = ((lights[j]->position() + offset) - intersection).normalize3();

                        if (!isShadowed(intersection, lights[j]->position(), shadowRay))
                        {
                            float distToLight = intersection.distance(lights[j]->position());

                            distToLight *= distToLight;

                            // c.r += lights[j]->intensity() * lights[j]->color().r / 255.0 * objectColor.r / distToLight;
                            // c.g += lights[j]->intensity() * lights[j]->color().g / 255.0 * objectColor.g / distToLight;
                            // c.b += lights[j]->intensity() * lights[j]->color().b / 255.0 * objectColor.b / distToLight;

                            shadowR += (weight * lights[j]->intensity() * lights[j]->color().r / 255.0 * 1 / distToLight) / 255;
                            shadowG += (weight * lights[j]->intensity() * lights[j]->color().g / 255.0 * 1 / distToLight) / 255;
                            shadowB += (weight * lights[j]->intensity() * lights[j]->color().b / 255.0 * 1 / distToLight) / 255;
                        }
                    }


                    c.r += shadowR * objectColor.r;
                    c.g += shadowG * objectColor.g;
                    c.b += shadowB * objectColor.b;
                }


                Vector directionalLight = 100 * Vector(-DIRECTIONAL_LIGHT_DIRECTION_X, -DIRECTIONAL_LIGHT_DIRECTION_Y, -DIRECTIONAL_LIGHT_DIRECTION_Z);

                Vector directionalLightShadowRay = (directionalLight - intersection).normalize3();

                if (!isShadowed(intersection, directionalLight, directionalLightShadowRay))
                {
                    c.r += DIRECTIONAL_LIGHT_INTENSITY * DIRECTIONAL_LIGHT_COLOR.r / 255.0 * objectColor.r;
                    c.g += DIRECTIONAL_LIGHT_INTENSITY * DIRECTIONAL_LIGHT_COLOR.g / 255.0 * objectColor.g;
                    c.b += DIRECTIONAL_LIGHT_INTENSITY * DIRECTIONAL_LIGHT_COLOR.b / 255.0 * objectColor.b;
                }

                return c;
            }
        }
    }

    return SKY_COLOR;
}

bool Renderer::isShadowed(Vector origin, Vector light, Vector ray)
{
    //Vector ray = (light - origin).normalize3();

    Vector originMoved = origin + ray * STEP_SIZE;

    Vector curr = originMoved;
    Vector prev = originMoved;

    // UV uvOrigin = this->VectorToUV(originMoved);
    // UV uvRay = this->VectorToUV(ray);

    UV uvPrev = this->VectorToUV(origin);
    UV uvCurr = this->VectorToUV(origin);
    UV uvOrigin = this->VectorToUV(origin);
    UV uvRay = this->VectorToUV(ray);

    for (float h = 1; h < MAX_ITER; h += 1)
    {
        switch (SPACE_TYPE)
        {
        case EUCLIDEAN:
            prev = curr;
            curr = originMoved + ray * h * STEP_SIZE;
            break;

        case FLAT_TORUS:
            prev = curr;
            curr = originMoved + ray * h * STEP_SIZE;

            curr.x = mapToFundamentalDomain(curr.x, FUNDAMENTAL_DOMAIN_X_MIN, FUNDAMENTAL_DOMAIN_X_MAX);
            curr.y = mapToFundamentalDomain(curr.y, FUNDAMENTAL_DOMAIN_Y_MIN, FUNDAMENTAL_DOMAIN_Y_MAX);
            curr.z = mapToFundamentalDomain(curr.z, FUNDAMENTAL_DOMAIN_Z_MIN, FUNDAMENTAL_DOMAIN_Z_MAX);
            break;

        case MIRRORED_CUBE:
        {
            prev = curr;
            curr = originMoved + ray * h * STEP_SIZE;

            Vector newCurr = Vector(0, 0, 0);

            newCurr.x = mapToFundamentalDomain(curr.x, FUNDAMENTAL_DOMAIN_X_MIN, FUNDAMENTAL_DOMAIN_X_MAX);
            newCurr.y = mapToFundamentalDomain(curr.y, FUNDAMENTAL_DOMAIN_Y_MIN, FUNDAMENTAL_DOMAIN_Y_MAX);
            newCurr.z = mapToFundamentalDomain(curr.z, FUNDAMENTAL_DOMAIN_Z_MIN, FUNDAMENTAL_DOMAIN_Z_MAX);

            if (newCurr != curr)
            {
                newCurr = -1 * newCurr;
            }

            curr = newCurr;
        }
        break;

        case SPHERICAL:
            prev = curr;
            curr = originMoved + ray * h * STEP_SIZE;

            // uvPrev = uvCurr;
            // prev = curr;

            // uvRay = this->rungeKutta4(this->VectorToUV(prev), uvRay, STEP_SIZE);

            // Vector rayXYZ = this->UVToVector(uvRay);

            // curr = prev + rayXYZ * STEP_SIZE;

            // uvCurr.u = uvPrev.u + uvRay.u * STEP_SIZE;
            // uvCurr.v = uvPrev.v + uvRay.v * STEP_SIZE;

            break;
        }

        if (curr.distance(light) < 0.1)
            return false;

        for (int i = 0; i < this->numObjects; i++)
            if (objects[i]->intersect(prev, curr) && objects[i]->translucency() == 0)
                return true;
    }

    return false;
}

UV F(UV x, UV y)
{
    return {
        cos(x.u) * sin(x.u) * y.v * y.v,
        -2 * atan(x.u) * y.u * y.v};

    // return {
    //     cos(x.u) * sin(x.u) * y.v * y.v,
    //     -2 * (cos(x.u) / (sin(x.u) + 0.00001)) * y.u * y.v};
}

UV Renderer::rungeKutta4(UV x, UV y, float t)
{

    UV xNew = {
        x.u + (double)t * y.u,
        x.v + (double)t * y.v};

    UV accelaration = F(xNew, y);

    UV yNew = {
        y.u + (double)t * accelaration.u,
        y.v + (double)t * accelaration.v};

    return yNew;
}

UV Renderer::VectorToUV(Vector v)
{
    UV uv;

    // float theta, phi;

    // theta = acos(mapToFundamentalDomain(v.z, -1, 1));
    // phi = atan2(mapToFundamentalDomain(v.y, -1, 1), mapToFundamentalDomain(v.x, -1, 1));

    // v = v.normalize3();

    // theta = acos(v.z/SPHERICAL_SPACE_RADIUS);
    // phi = atan2(v.y, v.x);

    sphereRadius = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    uv.u = acos(v.z / sphereRadius);
    uv.v = atan2(v.y, v.x);

    // uv.u = mapToFundamentalDomain(uv.u, -1, 1);
    // uv.v = mapToFundamentalDomain(uv.v, -1, 1);

    // uv.u = (phi + PI) / (2 * PI);
    // uv.v = (theta + PI/2) / PI;

    return uv;

    // UV uv;

    // float uArg = v.z / SPHERICAL_SPACE_RADIUS;
    // uArg = mapToFundamentalDomain(uArg, -1, 1);
    // uv.u = acos(uArg);

    // if (v.y < 0)
    //     uv.u = 2 * PI - uv.u;

    // float vArg = v.x / (SPHERICAL_SPACE_RADIUS * sin(uv.u));
    // vArg = mapToFundamentalDomain(vArg, -1, 1);
    // uv.v = acos(vArg);

    // return uv;
}

Vector Renderer::UVToVector(UV uv)
{

    // float theta, phi;

    // theta = uv.v;
    // phi = uv.u;

    // return Vector(
    //     sin(theta) * cos(phi) / SPHERICAL_SPACE_RADIUS,
    //     sin(theta) * sin(phi) / SPHERICAL_SPACE_RADIUS,
    //     cos(theta) / SPHERICAL_SPACE_RADIUS
    // );

    // return Vector(
    //     sin(uv.u) * cos(uv.v) / SPHERICAL_SPACE_RADIUS,
    //     sin(uv.u) * sin(uv.v) / SPHERICAL_SPACE_RADIUS,
    //     cos(uv.u) / SPHERICAL_SPACE_RADIUS);

    return Vector(
        sphereRadius * sin(uv.u) * cos(uv.v),
        sphereRadius * sin(uv.u) * sin(uv.v),
        sphereRadius * cos(uv.u));

    // float theta, phi;

    // phi = PI * (uv.v - 0.5);
    // theta = 2 * PI * uv.u - PI;

    // return Vector(
    //     SPHERICAL_SPACE_RADIUS * sin(phi) * cos(theta),
    //     SPHERICAL_SPACE_RADIUS * sin(phi) * sin(theta),
    //     SPHERICAL_SPACE_RADIUS * cos(phi));
}

void Renderer::initExecutionTime()
{
    this->executionStart = std::chrono::high_resolution_clock::now();
}