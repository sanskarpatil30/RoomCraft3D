#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "camera.h"
#include "room.h"
#include "scene.h"

class InputHandler {
public:
    static bool isLeftDragging;
    static bool isRightDragging;
    static int lastMouseX;
    static int lastMouseY;

    static void handleKeyboard(unsigned char key, int x, int y, Scene& scene, Room& room, Camera& camera);
    static void handleSpecialKeys(int key, int x, int y, Scene& scene, Room& room, Camera& camera);
    static void handleMouse(int button, int state, int x, int y, Scene& scene, Camera& camera);
    static void handleMotion(int x, int y, Camera& camera);
};

#endif // INPUT_H
