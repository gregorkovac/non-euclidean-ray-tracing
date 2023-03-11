#include <GLFW/glfw3.h>

#include "../include/ParametersAndConstants.h"
#include "../include/Renderer.h"

using namespace std;

Renderer* renderer = nullptr;

int main(void)
{
    GLFWwindow* window;

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

    renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT, PIXEL_SIZE);

    unsigned char data[WINDOW_WIDTH * WINDOW_HEIGHT * 3];

    for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT * 3; i++) {
        data[i] = 0;
    }

    renderer->render(data);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}