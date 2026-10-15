#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Camera projection modes
enum class CameraMode {
    PERSPECTIVE_ORBIT,  // Free 3D orbit around room center
    TOP_DOWN_ORTHO,     // Overhead architectural 2D blueprint view
    FRONT_VIEW,         // Direct front elevation
    SIDE_VIEW           // Direct side elevation
};

class Camera {
public:
    CameraMode mode;

    // Orbit parameters (Spherical coordinates)
    float yaw;          // Angle in degrees around Y-axis (azimuth)
    float pitch;        // Angle in degrees above XZ-plane (elevation)
    float distance;     // Distance from target point

    // Target look-at point (usually center of the room)
    float targetX;
    float targetY;
    float targetZ;

    // Viewport dimensions
    int viewportWidth;
    int viewportHeight;

    // Orthographic view zoom factor
    float orthoSize;

    Camera();

    void reset();
    void setMode(CameraMode newMode);
    void orbit(float deltaYaw, float deltaPitch);
    void zoom(float deltaDistance);
    void pan(float deltaX, float deltaZ);
    void updateViewport(int width, int height);

    // Applies projection matrix (glMatrixMode(GL_PROJECTION))
    void applyProjection() const;

    // Applies view matrix (glMatrixMode(GL_MODELVIEW) with gluLookAt)
    void applyView() const;

    // Get current eye position in world space
    void getEyePosition(float& eyeX, float& eyeY, float& eyeZ) const;
};

#endif // CAMERA_H
