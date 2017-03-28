#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  3.0f;
const GLfloat MOVE_SENSITIVTY =  0.1f;
const GLfloat SCROLL_SENSITIVTY =  0.2f;
const GLfloat ZOOM       =  45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;
    // Eular Angles
    GLfloat mYaw;
    GLfloat mPitch;
    // Camera options
    GLfloat mMovementSpeed;
    GLfloat mZoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 getViewMatrix();

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(Camera_Movement direction, GLfloat deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(GLfloat yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();
};
