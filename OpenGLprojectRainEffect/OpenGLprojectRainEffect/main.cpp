#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>

// Vertex shader
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    void main() {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    }
)";

// Fragment shader
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(0.7f, 0.7f, 1.0f, 1.0f);  // Light blue color
    }
)";

// Raindrop structure
struct Raindrop {
    float x, y;    // Position
    float speed;   // Falling speed
    float length;  // Length of the raindrop
};

// Global variables
std::vector<Raindrop> raindrops;
const int NUM_RAINDROPS = 100;
float WINDOW_WIDTH = 800;
float WINDOW_HEIGHT = 600;

// Random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> xDist(-1.0f, 1.0f);
std::uniform_real_distribution<float> speedDist(0.005f, 0.015f);
std::uniform_real_distribution<float> lengthDist(0.02f, 0.04f);

// Initialize raindrop
void initRaindrop(Raindrop& drop, bool randomY = true) {
    drop.x = xDist(gen);
    drop.y = randomY ? std::uniform_real_distribution<float>(-1.0f, 1.0f)(gen) : 1.0f;
    drop.speed = speedDist(gen);
    drop.length = lengthDist(gen);
}

// Error callback
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Check for OpenGL errors
void checkGLError(const char* operation) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error after " << operation << ": " << error << std::endl;
    }
}

// Compile and link shaders
unsigned int createShaderProgram() {
    int success;
    char infoLog[512];

    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check vertex shader compilation
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
        return 0;
    }

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
        return 0;
    }

    // Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check program linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        return 0;
    }

    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set error callback
    glfwSetErrorCallback(errorCallback);

    // Create window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GLFW Rain Effect", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make OpenGL context current
    glfwMakeContextCurrent(window);

#if !defined(__APPLE__)
    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }
#endif

    // Initialize raindrops
    raindrops.resize(NUM_RAINDROPS);
    for (auto& drop : raindrops) {
        initRaindrop(drop);
    }

    // Create and use shader program
    unsigned int shaderProgram = createShaderProgram();
    if (shaderProgram == 0) {
        glfwTerminate();
        return -1;
    }
    glUseProgram(shaderProgram);
    checkGLError("shader program creation");

    // Create vertex buffer and vertex array objects
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Setup vertex attributes (do this once)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    checkGLError("buffer setup");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update and render raindrops
        std::vector<float> vertices;
        vertices.reserve(NUM_RAINDROPS * 4); // Pre-allocate memory

        for (auto& drop : raindrops) {
            // Update position
            drop.y -= drop.speed;

            // Reset raindrop if it goes below screen
            if (drop.y + drop.length < -1.0f) {
                initRaindrop(drop, false);
            }

            // Add vertices for this raindrop
            vertices.push_back(drop.x);
            vertices.push_back(drop.y);
            vertices.push_back(drop.x);
            vertices.push_back(drop.y + drop.length);
        }

        // Update buffer data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        checkGLError("buffer update");

        // Draw raindrops
        glDrawArrays(GL_LINES, 0, vertices.size() / 2);
        checkGLError("draw");

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}