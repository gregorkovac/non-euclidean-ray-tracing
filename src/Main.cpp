#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "../include/ParametersAndConstants.h"
#include "../include/Renderer.h"

using namespace std;

Renderer *renderer = nullptr;

int main(int argc, char **argv)
{
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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
    
        /* Poll for and process events */
        glfwPollEvents();
    }

    printf("\033[0;35m\x1B[1m");
    printf("DONE!\n");
    printf("\x1B[0m\033[0m");

    glfwTerminate();

    printf("Terminated.\n");

    return 0;
}