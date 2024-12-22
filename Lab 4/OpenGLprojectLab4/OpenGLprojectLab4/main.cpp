//
//  main.cpp
//  OpenGL_Shader_Example_step1
//
//  Created by CGIS on 30/11/15.
//  Copyright © 2015 CGIS. All rights reserved.
//

#if defined (__APPLE__)
    #define GLFW_INCLUDE_GLCOREARB
#else
    #define GLEW_STATIC
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <string>
#include "Shader.hpp"

int glWindowWidth = 640;
int glWindowHeight = 480;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

//vertex coordinates in normalized device coordinates
//GLfloat vertexData[] = {
//        0.0f,	0.5f,	0.0f,
//        -0.5f, -0.5f,	0.0f,
//        0.5f, -0.5f,	0.0f
//};

GLfloat vertexData[] = {
    //vertex position and vertex color
    0.0f, 0.7f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f
};

GLuint vertexIndices[] = {
    0, 1, 4,
    1, 2, 3,
    1, 3, 4
};

GLuint verticesVBO;
GLuint verticesEBO;
GLuint objectVAO;

gps::Shader myCustomShader;

GLint uniformXLocation;
GLint uniformYLocation;

float valUniformX = 0;
float valUniformY = 0;

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
    //TODO
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        
        glfwSetWindowShouldClose(glWindow, GLFW_TRUE);
    }

    if (glfwGetKey(glWindow, GLFW_KEY_A) == GLFW_PRESS) {
        valUniformX -= 0.1;
        glUniform1f(uniformXLocation, valUniformX);
        glUniform1f(uniformYLocation, valUniformY);
    }

    if (glfwGetKey(glWindow, GLFW_KEY_D) == GLFW_PRESS) {
        valUniformX += 0.1;
        glUniform1f(uniformXLocation, valUniformX);
        glUniform1f(uniformYLocation, valUniformY);
    }

    if (glfwGetKey(glWindow, GLFW_KEY_W) == GLFW_PRESS) {
        valUniformY += 0.1;
        glUniform1f(uniformXLocation, valUniformX);
        glUniform1f(uniformYLocation, valUniformY);
    }

    if (glfwGetKey(glWindow, GLFW_KEY_S) == GLFW_PRESS) {
        valUniformY -= 0.1;
        glUniform1f(uniformXLocation, valUniformX);
        glUniform1f(uniformYLocation, valUniformY);
    }
}

void initObjects()
{
    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);

    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &verticesEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, verticesEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndices), vertexIndices, GL_STATIC_DRAW);

    //vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //vertex colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

bool initOpenGLWindow()
{
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //window scaling for HiDPI displays
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    // for multisampling/antialising
    glfwWindowHint(GLFW_SAMPLES, 4);

    glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
    if (!glWindow) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        return false;
    }

    glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
    glfwSetKeyCallback(glWindow, key_callback);
    glfwMakeContextCurrent(glWindow);

#if not defined (__APPLE__)
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
#endif

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    //for RETINA display
    glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

    return true;
}

void renderScene()
{
    glClearColor(0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, retina_width, retina_height);
       

    if (glfwGetKey(glWindow, GLFW_KEY_Q) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (glfwGetKey(glWindow, GLFW_KEY_E) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    myCustomShader.useShaderProgram();

    glBindVertexArray(objectVAO);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, sizeof(vertexIndices), GL_UNSIGNED_INT, 0);
}

void cleanup() {

    if (verticesVBO) {
        glDeleteBuffers(1, &verticesVBO);
    }

    if (objectVAO) {
        glDeleteVertexArrays(1, &objectVAO);
    }

    glfwDestroyWindow(glWindow);
    //close GL context and any other GLFW resources
    glfwTerminate();
}

int main(int argc, const char * argv[]) {

    if (!initOpenGLWindow()) {
        glfwTerminate();
        return 1;
    }

    initObjects();

    myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
    myCustomShader.useShaderProgram();

    //folosire uniform
    //GLint uniformColourLocation = glGetUniformLocation(myCustomShader.shaderProgram, "uniformColour");
    //glUniform3f(uniformColourLocation, 0.15f, 0.0f, 0.84f);

    uniformXLocation = glGetUniformLocation(myCustomShader.shaderProgram, "uniformX");
    glUniform1f(uniformXLocation, valUniformX);
    uniformYLocation = glGetUniformLocation(myCustomShader.shaderProgram, "uniformY");
    glUniform1f(uniformYLocation, valUniformY);

    while (!glfwWindowShouldClose(glWindow)) {
        renderScene();

        glfwPollEvents();
        glfwSwapBuffers(glWindow);
    }

    cleanup();

    return 0;
}