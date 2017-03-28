// Local Headers
#include "common.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "mesh.hpp"

// Standard Headers
#include <cstdio>
#include <cstdlib>

using namespace Common;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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

    // Set the required callback functions
    glfwSetKeyCallback(mWindow, key_callback);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);
    // Options
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL();
    glfwGetFramebufferSize(mWindow, &w, &h);
    glViewport(0, 0, w, h);

    Shader objectShader;
    objectShader.attach(__FILE__, "object.vert");
    objectShader.attach(__FILE__, "object.frag");
    objectShader.link();
    Shader lightShader;
    lightShader.attach(__FILE__, "light.vert");
    lightShader.attach(__FILE__, "light.frag");
    lightShader.link();

    GLfloat vertices[] = {
        // Positions
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    GLuint lightVAO, VBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glm::vec3 lightPos(0.0f, 0.8f, 2.0f);
	Mesh nanosuit(__FILE__, "nanosuit.obj");
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader.activate();
        // Create transformations
        glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        glm::mat4 view;
        view = camera.getViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.mZoom), (GLfloat)mWidth / (GLfloat)mHeight, 0.1f, 1000.0f);
        // Pass them to the shaders
        glUniformMatrix4fv(glGetUniformLocation(objectShader.get(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(objectShader.get(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(objectShader.get(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		nanosuit.draw(objectShader.get());

        lightShader.activate();
        model = glm::mat4();
        // Light attributes
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.get(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.get(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.get(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
    }
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return EXIT_SUCCESS;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.processKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.processKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.processKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.processKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}
