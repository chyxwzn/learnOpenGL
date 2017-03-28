#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
      mMovementSpeed(SPEED),
      mZoom(ZOOM)
{
    mPosition = position;
    mWorldUp = up;
    mYaw = yaw;
    mPitch = pitch;
    updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
    : mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
      mMovementSpeed(SPEED),
      mZoom(ZOOM)
{
    mPosition = glm::vec3(posX, posY, posZ);
    mWorldUp = glm::vec3(upX, upY, upZ);
    mYaw = yaw;
    mPitch = pitch;
    updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = mMovementSpeed * deltaTime;
    if (direction == FORWARD)
        mPosition += mFront * velocity;
    if (direction == BACKWARD)
        mPosition -= mFront * velocity;
    if (direction == LEFT)
        mPosition -= mRight * velocity;
    if (direction == RIGHT)
        mPosition += mRight * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset = glm::abs(xoffset) > glm::abs(yoffset) ? xoffset * MOVE_SENSITIVTY : 0.0f;
    yoffset = glm::abs(xoffset) > glm::abs(yoffset) ? 0.0f : yoffset * MOVE_SENSITIVTY;

    mYaw   += xoffset;
    mPitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (mPitch > 89.0f)
            mPitch = 89.0f;
        if (mPitch < -89.0f)
            mPitch = -89.0f;
    }

    // Update Front, mRight and mUp Vectors using the updated Eular angles
    updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::processMouseScroll(GLfloat yoffset)
{
    yoffset *= SCROLL_SENSITIVTY;
    if (mZoom >= 1.0f && mZoom <= 89.0f)
        mZoom -= yoffset;
    if (mZoom <= 1.0f)
        mZoom = 1.0f;
    if (mZoom >= 89.0f)
        mZoom = 89.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);
    // Also re-calculate the mRight and mUp vector
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    mUp    = glm::normalize(glm::cross(mRight, mFront));
}
