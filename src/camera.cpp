#include "camera.h"
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <algorithm>

Camera::Camera() {
    viewportWidth = 1024;
    viewportHeight = 768;
    reset();
}

void Camera::reset() {
    mode = CameraMode::PERSPECTIVE_ORBIT;
    yaw = 45.0f;          // 45 degrees angle for isometric-like perspective
    pitch = 30.0f;        // 30 degrees elevation looking down into the room
    distance = 18.0f;     // Far enough to frame a 10x10 room comfortably
    targetX = 0.0f;
    targetY = 2.0f;       // Mid-height of room
    targetZ = 0.0f;
    orthoSize = 7.5f;     // Half-extent for orthographic view
}

void Camera::setMode(CameraMode newMode) {
    mode = newMode;
    if (mode == CameraMode::TOP_DOWN_ORTHO) {
        orthoSize = 7.5f;
    }
}

void Camera::orbit(float deltaYaw, float deltaPitch) {
    if (mode != CameraMode::PERSPECTIVE_ORBIT) return;

    yaw += deltaYaw;
    pitch += deltaPitch;

    // Clamp pitch so camera doesn't flip upside down
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < 5.0f)  pitch = 5.0f;   // Don't go below floor level

    // Keep yaw within [0, 360)
    while (yaw >= 360.0f) yaw -= 360.0f;
    while (yaw < 0.0f) yaw += 360.0f;
}

void Camera::zoom(float deltaDistance) {
    if (mode == CameraMode::PERSPECTIVE_ORBIT) {
        distance += deltaDistance;
        if (distance < 4.0f) distance = 4.0f;
        if (distance > 40.0f) distance = 40.0f;
    } else {
        orthoSize += deltaDistance * 0.4f;
        if (orthoSize < 2.0f) orthoSize = 2.0f;
        if (orthoSize > 25.0f) orthoSize = 25.0f;
    }
}

void Camera::pan(float deltaX, float deltaZ) {
    targetX += deltaX;
    targetZ += deltaZ;
}

void Camera::updateViewport(int width, int height) {
    viewportWidth = (width > 0) ? width : 1;
    viewportHeight = (height > 0) ? height : 1;
    glViewport(0, 0, viewportWidth, viewportHeight);
}

void Camera::getEyePosition(float& eyeX, float& eyeY, float& eyeZ) const {
    if (mode == CameraMode::PERSPECTIVE_ORBIT) {
        // Spherical to Cartesian coordinate conversion:
        // pitch is elevation above horizontal XZ plane
        // yaw is rotation around vertical Y axis
        float radYaw = yaw * (float)M_PI / 180.0f;
        float radPitch = pitch * (float)M_PI / 180.0f;

        eyeX = targetX + distance * cos(radPitch) * sin(radYaw);
        eyeY = targetY + distance * sin(radPitch);
        eyeZ = targetZ + distance * cos(radPitch) * cos(radYaw);
    } else if (mode == CameraMode::TOP_DOWN_ORTHO) {
        eyeX = targetX;
        eyeY = targetY + 25.0f;
        eyeZ = targetZ;
    } else if (mode == CameraMode::FRONT_VIEW) {
        eyeX = targetX;
        eyeY = targetY;
        eyeZ = targetZ + distance;
    } else { // SIDE_VIEW
        eyeX = targetX + distance;
        eyeY = targetY;
        eyeZ = targetZ;
    }
}

void Camera::applyProjection() const {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)viewportWidth / (float)viewportHeight;

    if (mode == CameraMode::PERSPECTIVE_ORBIT || mode == CameraMode::FRONT_VIEW || mode == CameraMode::SIDE_VIEW) {
        // Perspective Projection:
        // gluPerspective(FieldOfViewY, AspectRatio, NearClippingPlane, FarClippingPlane)
        gluPerspective(45.0, aspect, 0.1, 100.0);
    } else {
        // Orthographic 2D Blueprint Projection:
        // glOrtho(left, right, bottom, top, near, far)
        if (aspect >= 1.0f) {
            glOrtho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, 1.0, 100.0);
        } else {
            glOrtho(-orthoSize, orthoSize, -orthoSize / aspect, orthoSize / aspect, 1.0, 100.0);
        }
    }

    glMatrixMode(GL_MODELVIEW);
}

void Camera::applyView() const {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float eyeX, eyeY, eyeZ;
    getEyePosition(eyeX, eyeY, eyeZ);

    if (mode == CameraMode::PERSPECTIVE_ORBIT) {
        gluLookAt(eyeX, eyeY, eyeZ,
                  targetX, targetY, targetZ,
                  0.0, 1.0, 0.0); // Up vector is +Y
    } else if (mode == CameraMode::TOP_DOWN_ORTHO) {
        // Looking straight down onto the XZ floor, so up vector is -Z
        gluLookAt(eyeX, eyeY, eyeZ,
                  targetX, targetY, targetZ,
                  0.0, 0.0, -1.0);
    } else if (mode == CameraMode::FRONT_VIEW) {
        gluLookAt(eyeX, eyeY, eyeZ,
                  targetX, targetY, targetZ,
                  0.0, 1.0, 0.0);
    } else { // SIDE_VIEW
        gluLookAt(eyeX, eyeY, eyeZ,
                  targetX, targetY, targetZ,
                  0.0, 1.0, 0.0);
    }
}
