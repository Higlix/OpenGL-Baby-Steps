#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// SHADER IS A PROGRAM THAT RUNS IN THE GPU

#define VERTEX_SHADER_PATH "res/shaders/vertex_shader.shader"
#define FRAGMENT_SHADER_PATH "res/shaders/fragment_shader.shader"

std::string gcReadVertexShader()
{
    std::string vertexShader;
    char file[10000] = { 0 };
    int fd = open(VERTEX_SHADER_PATH, O_RDONLY);
    int bytes = 1;
    int i = 0;

    while (bytes)
    {
        bytes = read(fd, &file[i], 1);
        if (bytes == -1)
        {
            close(fd);
            return ("");
        }
        i++;
    }
    close(fd);
    vertexShader.append(file);
    return (vertexShader);
}
std::string gcReadFragmentShader()
{
    std::string fragmentShader;
    char file[10000] = { 0 };
    int fd = open(FRAGMENT_SHADER_PATH, O_RDONLY);
    int bytes = 1;
    int i = 0;

    while (bytes)
    {
        bytes = read(fd, &file[i], 1);
        if (bytes == -1)
        {
            close(fd);
            return ("");
        }
        i++;
    }
    close(fd);
    fragmentShader.append(file);
    return (fragmentShader);
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = &source[0];
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) // GL_FALSE = 0
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        if (type == GL_VERTEX_SHADER)
            std::cout << "Failed to compile vertex shader :" << message << std::endl;
        else
            std::cout << "Failed to compile fragment shader :" << message << std::endl;
        glDeleteShader(id);
        return (0);
    }
    // TODO: ERROR HANDLE

    return (id);
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);


    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return (program);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1200, 900, "Graphics", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
  
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

        // X   // Y
    float positions[] = {
        -0.5f,-0.5f,
         0.5f,-0.5f,
         0.5f, 0.5f,
        -0.5f, 0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3 ,0
    };

    unsigned int buffer;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    unsigned int ibo;

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    // std::string vertexShader = gcReadVertexShader();
    // std::cout << vertexShader << std::endl;
    // std::string fragmentShader = gcReadFragmentShader();
    // std::cout << fragmentShader << std::endl;

    unsigned int shader = CreateShader(gcReadVertexShader(), gcReadFragmentShader());
    glUseProgram(shader);

    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        
        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glDeleteShader(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}