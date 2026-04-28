#include <iostream>
#include <cmath>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"
#include "timeUtils.h"

#define TAN_PI_6 (float(std::sqrt(3)) / 3)

// set up vertex data and buffer(s) and configure vertex attributes
GLfloat vertices[] =
{   // vertex coords        // colors           // uv
    -0.5f, -0.5f, 0.0f,     0.9f, 0.1f, 0.1f,   0.0f, 0.0f, // lower left
    -0.5f,  0.5f, 0.0f,     0.9f, 0.9f, 0.1f,   0.0f, 1.0f, // upper left
     0.5f,  0.5f, 0.0f,     0.1f, 0.9f, 0.1f,   1.0f, 1.0f, // upper right
     0.5f, -0.5f, 0.0f,     0.1f, 0.1f, 0.9f,   1.0f, 0.0f // lower right
};
GLuint indices[] =
{
    0, 2, 1,
    0, 3, 2
};

// Declare functions to implement later on
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCREEN_WIDTH = 600;
const unsigned int SCREEN_HEIGHT = 600;

int main()
{
    ClockUtil myClock;

    // "GLFW is an Open Source, multi-platform library for OpenGL, 
    // OpenGL ES and Vulkan development on the desktop. It provides 
    // a simple API for creating windows, contexts and surfaces, 
    // receiving input and events." -- www.glfw.org

    // Initialize GLFW, set OpenGL version 4.6, use core profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a window object of 800x600 pixels with title LearnOpenGL
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Register framebuffer resize callback function
    // Always register callback functions after creating the
    // window and before render loop initiation
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD manages function pointers for OpenGL
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell OpenGL the size of the rendering window
    // First two params: location of lower-left corner of window
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Get id of uniform transform
    GLuint unifTransID = glGetUniformLocation(shaderProgram.ID, "transform");

    // Time handling
    time_t timer;
    GLuint unifTimeID = glGetUniformLocation(shaderProgram.ID, "time");
    GLuint unifDeltaTimeID = glGetUniformLocation(shaderProgram.ID, "deltaTime");

    // Texture
    Texture texture("brainrot.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    texture.texUnit(shaderProgram, "tex0", 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands

        // Set intended background color
        glClearColor(0.02f, 0.1f, 0.1f, 1.0f);   // set state
        // Clean back buffer and assign new color to it
        glClear(GL_COLOR_BUFFER_BIT);            // use state
        // Tell OpenGL which shader program we want to use
        shaderProgram.Activate();
        

        // Identity matrix-4
        glm::mat4 trans = glm::mat4(1.0f);
        // Translation happens second mathematically
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // Rotation happens second mathematically
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(unifTransID, 1, GL_FALSE, glm::value_ptr(trans));
        glUniform1f(unifTimeID, (GLfloat)(myClock.getTimeSinceInit()));
        glUniform1f(unifDeltaTimeID, (GLfloat)(myClock.deltaTime()));

        std::cout << "Current time: " << myClock.getTimeSinceInit() << "\nTime as GLfloat: " << (GLfloat)(myClock.getTimeSinceInit()) << std::endl
        << "Delta time: " << myClock.deltaTime() << std::endl;

        texture.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, index data type, index of indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // unbind

        // See double buffer (front and back)
        glfwSwapBuffers(window);
        glfwPollEvents();

        myClock.update();
    }

    // deallocate all resources once they have outlived their purpose
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    texture.Delete();
    shaderProgram.Delete();

    // glfw: terminate, clearning all previously allocated GLFW resources.
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// Resize viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Close on escape
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}