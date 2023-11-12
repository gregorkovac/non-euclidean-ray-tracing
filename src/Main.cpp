#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "../include/ParametersAndConstants.h"
#include "../include/Renderer.h"

using namespace std;

Renderer *renderer = nullptr;

#include "../include/Vector4.h"
#include <math.h>
#include <vector>

Vector4 funct(Vector4 x) {
    return Vector4(x.y, cosl(x.x) * sinl(x.x)*x.w*x.w, x.w, -2*1/tanl(x.x)*x.y*x.w);
}

void plotPoints(vector<Vector> points) {
    FILE *f = fopen("points.txt", "w");
    for (auto point : points) {
        fprintf(f, "%f %f %f\n", point.x, point.y, point.z);
    }
    fclose(f);

    system("python3 other/plot_points.py");
}

int main(int argc, char **argv)
{
  /* double u = 1.570796;
    double v = 1;
    double w = 0;
    double z = 0.1;

    double t0 = 0;
    Vector4 x0 = Vector4(u, v, w, z);
    double b = 10;
    double epsilon = 1E-6;
    double sigma = 0.9;

    vector<Vector4> X = DOPRI5(funct, t0, x0, b, epsilon, sigma);

    vector<Vector> points = vector<Vector>();
    for (int i = 0; i < X.size(); i++) {
        double u = X[i].x;
        double v = X[i].z;

        Vector pt = Vector(sin(u) * cos(v), sin(u)*sin(v), cos(u));
        points.push_back(pt);
    }

    plotPoints(points); */

    srand(time(NULL));

    if (PRINT_PROJECT_INFO)
    {
        printf("\033[0;35m\x1B[1m");
        printf("-----------------------------\n");
        printf("-----------------------------\n");
        printf("- NON EUCLIDEAN RAY TRACING -\n");
        printf("------ Diploma thesis -------\n");
        printf("-----------------------------\n");
        printf("\033[0;35m\x1B[0m");
        printf("        Gregor Kovač \n\n");
        printf("  Faculty of computer and \n");
        printf("    information science, \n");
        printf("  University of Ljubljana\n\n");
        printf("      Ljubljana, 2023\n");
        printf("\033[0;35m\x1B[1m");
        printf("-----------------------------\n");
        printf("-----------------------------\n\n\n");
        printf("\033[0;35m\x1B[0m");
    }

    // printf("\033[0;36m");
    printf("Setting up window...\n");

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Non Euclidean Ray Tracing", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    glViewport(0, 0, frameWidth, frameHeight);

    renderer = new Renderer(frameWidth, frameHeight);

    printf("Parsing scene...\n");

    if (argc > 1)
        renderer->parseScene(argv[1]);
    else
        renderer->parseScene("./scenes/scene1.txt");

    printf("Rendering...\n");
    unsigned char data[frameWidth * frameHeight * 3];

    for (int i = 0; i < frameWidth * frameHeight * 3; i++)
    {
        data[i] = 0;
    }

    unsigned char newData[frameWidth * frameHeight * 3];

    renderer->initExecutionTime();

    for (int i = 0; i < SUB_IMAGE_COUNT; i++)
    {
        if (i == 0)
        {
            renderer->render(data);
        }
        else
        {
            renderer->render(newData);
            for (int j = 0; j < frameWidth * frameHeight * 3; j++)
            {
                data[j] = (data[j] + newData[j]) / 2;
            }
        }

        if (DRAW_IMAGE)
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            glDrawPixels(frameWidth, frameHeight, GL_RGB, GL_UNSIGNED_BYTE, data);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            glfwPollEvents();
        }

        printf("Rendered sub-image %d/%d\n", i + 1, SUB_IMAGE_COUNT);
    }

    if (argc > 2 && strcmp(argv[2], "--save") == 0)
    {

        char imagePath[100];
        if (argc > 3)
            strcpy(imagePath, argv[3]);
        else
            strcpy(imagePath, "./renders/render.png");

        stbi_write_png(imagePath, frameWidth, frameHeight, 3, data, frameWidth * 3);

        printf("Saved image to %s\n", imagePath);
    }

    printf("\033[0;35m\x1B[1m");
    printf("DONE!\n");
    printf("\x1B[0m\033[0m");

    if (DRAW_IMAGE)
    {
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            /* Poll for and process events */
            glfwPollEvents();
        }

        glfwTerminate();
    }

    printf("Terminated.\n");

    return 0;
}