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

    if (PRINT_PROJECT_INFO) {
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

    //printf("\033[0;36m");
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

    renderer->render(data);

    if (argc > 2 && strcmp(argv[2], "--save") == 0) {
        printf("0\n");

        char imagePath[100];
        if (argc > 3)
            strcpy(imagePath, argv[3]);
        else
            strcpy(imagePath, "./render.png");
        
        printf("1\n");

        // Reverse the image
        unsigned char dataReverse[frameWidth * frameHeight * 3];
        for (int i = 0; i < frameHeight; i++) {
            for (int j = 0; j < frameWidth; j++) {
                dataReverse[i * frameWidth * 3 + j * 3 + 0] = data[(frameHeight - i - 1) * frameWidth * 3 + j * 3 + 0];
                dataReverse[i * frameWidth * 3 + j * 3 + 1] = data[(frameHeight - i - 1) * frameWidth * 3 + j * 3 + 1];
                dataReverse[i * frameWidth * 3 + j * 3 + 2] = data[(frameHeight - i - 1) * frameWidth * 3 + j * 3 + 2];
            }
        }

        printf("2\n");

        stbi_write_png(imagePath, frameWidth, frameHeight, 3, dataReverse, frameWidth * 3);

        printf("Saved image to %s\n", imagePath);
    }

    printf("\033[0;35m\x1B[1m");
    printf("DONE!\n");
    printf("\x1B[0m\033[0m");

    /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawPixels(frameWidth, frameHeight, GL_RGB, GL_UNSIGNED_BYTE, data);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    printf("Terminated.\n");

    return 0;
}