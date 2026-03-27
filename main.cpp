#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>

float vertices[] =
{
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    // "GLFW is an Open Source, multi-platform library for OpenGL, 
    // OpenGL ES and Vulkan development on the desktop. It provides 
    // a simple API for creating windows, contexts and surfaces, 
    // receiving input and events." -- www.glfw.org

    // Initialize GLFW, set OpenGL version 4.6, use core profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window object of 800x600 pixels with title LearnOpenGL
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD manages function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell OpenGL the size of the rendering window
    // First two params: location of lower-left corner of window
    glViewport(0, 0, 800, 600);

    // Register framebuffer resize callback function
    // Always register callback functions after creating the
    // window and before render loop initiation
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands
        glClearColor(1.0f, 0.8f, 0.1f, 1.0f);   // set state
        glClear(GL_COLOR_BUFFER_BIT);           // use state

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // See double buffer (front and back)
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}