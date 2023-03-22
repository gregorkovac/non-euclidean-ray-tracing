#include <GLFW/glfw3.h>

#include "../include/ParametersAndConstants.h"
#include "../include/Renderer.h"

using namespace std;

Renderer *renderer = nullptr;

int main(void)
{
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

    printf("Rendering...\n");

    renderer = new Renderer(frameWidth, frameHeight);

    unsigned char data[frameWidth * frameHeight * 3];

    for (int i = 0; i < frameWidth * frameHeight * 3; i++)
    {
        data[i] = 0;
    }

    renderer->render(data);

    printf("Done!\n");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawPixels(frameWidth, frameHeight, GL_RGB, GL_UNSIGNED_BYTE, data);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    printf("Terminated.\n");

    return 0;
}