// Local Headers
#include "common.hpp"
#include "shader.hpp"

// Standard Headers
#include <cstdio>
#include <cstdlib>

using namespace Common;

int main(int argc, char * argv[]) {
    int w, h;

    // Load GLFW and Create a Window
    glfwInit();
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);
    // auto mWindow = glfwCreateWindow(mode->width, mode->height, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    glfwGetFramebufferSize(mWindow, &w, &h);
    glViewport(0, 0, w, h);

    Shader shader;
    shader.attach(__FILE__, "shader.vert");
    shader.attach(__FILE__, "shader.frag");
    shader.link();

    GLuint VAO, VBO;
    GLfloat vertices[] = {
        -0.85f, 0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // Top 
        -0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Right
        -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f // Left 
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    // Rendering Loop
    GLfloat offset = 0.0f;
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        glClear(GL_COLOR_BUFFER_BIT);
        shader.activate();
        GLuint location = glGetUniformLocation(shader.get(), "xOffset");
        offset += 0.01f;
        if (offset > 1.7f)
            offset = 0.0f;
        glUniform1f(location, offset);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return EXIT_SUCCESS;
}
