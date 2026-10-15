#include "input.h"
#include "save_load.h"
#include <GL/freeglut.h>
#include <iostream>

bool InputHandler::isLeftDragging = false;
bool InputHandler::isRightDragging = false;
int InputHandler::lastMouseX = 0;
int InputHandler::lastMouseY = 0;

void InputHandler::handleKeyboard(unsigned char key, int x, int y, Scene& scene, Room& room, Camera& camera) {
    float moveStep = 0.25f;

    switch (key) {
        case 27: // ESC key
            glutLeaveMainLoop();
            break;

        // --- Furniture Adding ---
        case '1':
            scene.addFurniture(FurnitureType::BED, room);
            break;
        case '2':
            scene.addFurniture(FurnitureType::CHAIR, room);
            break;
        case '3':
            scene.addFurniture(FurnitureType::TABLE, room);
            break;
        case '4':
            scene.addFurniture(FurnitureType::SOFA, room);
            break;
        case '5':
            scene.addFurniture(FurnitureType::WARDROBE, room);
            break;
        case '6':
            scene.addFurniture(FurnitureType::FLOOR_LAMP, room);
            break;

        // --- Selection ---
        case 9:   // TAB key
        case 32:  // SPACE key
            scene.selectNext();
            break;
        case '`': // Tilde / Backquote
            scene.selectPrev();
            break;
        case 'x':
        case 'X':
            scene.selectNone();
            break;

        // --- Movement (WASD) ---
        case 'w':
        case 'W':
            scene.moveSelected(0.0f, -moveStep, room); // Move North / -Z
            break;
        case 's':
        case 'S':
            scene.moveSelected(0.0f, moveStep, room);  // Move South / +Z
            break;
        case 'a':
        case 'A':
            scene.moveSelected(-moveStep, 0.0f, room); // Move West / -X
            break;
        case 'd':
        case 'D':
            scene.moveSelected(moveStep, 0.0f, room);  // Move East / +X
            break;

        // --- Rotation (Q / E) ---
        case 'q':
        case 'Q':
            scene.rotateSelected(-15.0f);
            break;
        case 'e':
        case 'E':
            scene.rotateSelected(15.0f);
            break;

        // --- Scaling ([ / ]) ---
        case '[':
            scene.scaleSelected(0.92f);
            break;
        case ']':
            scene.scaleSelected(1.08f);
            break;

        // --- Color Customization ---
        case 'c':
        case 'C':
            scene.cycleSelectedColor();
            break;

        // --- Duplicate Object ---
        case 'u':
        case 'U':
            scene.duplicateSelected(room);
            break;

        // --- Deletion (Backspace) ---
        case 8: // Backspace
        case 127: // Delete
            scene.deleteSelected();
            break;

        // --- Camera & Views ---
        case 'v':
        case 'V':
            if (camera.mode == CameraMode::PERSPECTIVE_ORBIT) {
                camera.setMode(CameraMode::TOP_DOWN_ORTHO);
                scene.setStatus("Camera: Top-Down Orthographic Blueprint");
            } else if (camera.mode == CameraMode::TOP_DOWN_ORTHO) {
                camera.setMode(CameraMode::FRONT_VIEW);
                scene.setStatus("Camera: Front Elevation View");
            } else if (camera.mode == CameraMode::FRONT_VIEW) {
                camera.setMode(CameraMode::SIDE_VIEW);
                scene.setStatus("Camera: Side Elevation View");
            } else {
                camera.setMode(CameraMode::PERSPECTIVE_ORBIT);
                scene.setStatus("Camera: 3D Perspective Orbit");
            }
            break;

        case 'r':
        case 'R':
            camera.reset();
            scene.setStatus("Camera reset to default orbit.");
            break;

        case '+':
        case '=':
            camera.zoom(-1.0f);
            break;
        case '-':
        case '_':
            camera.zoom(1.0f);
            break;

        // --- Room & Lighting Toggles ---
        case 'l':
        case 'L':
            Renderer::ceilingLightEnabled = !Renderer::ceilingLightEnabled;
            scene.setStatus(Renderer::ceilingLightEnabled ? "Ceiling Light: ON" : "Ceiling Light: OFF");
            break;
        case 'g':
        case 'G':
            room.showGrid = !room.showGrid;
            scene.setStatus(room.showGrid ? "Floor Grid: ON" : "Floor Grid: OFF");
            break;
        case 'f':
        case 'F':
            room.showFrontWall = !room.showFrontWall;
            scene.setStatus(room.showFrontWall ? "Front Wall: ON" : "Front Wall: OFF (Cutaway)");
            break;

        // --- Save / Load Operations ---
        case 'k':
        case 'K':
            if (SaveLoad::saveToFile("layouts/saved_room.txt", scene, room)) {
                scene.setStatus("Room saved to layouts/saved_room.txt");
            } else {
                scene.setStatus("Failed to save room.");
            }
            break;
        case 'o':
        case 'O':
            if (SaveLoad::loadFromFile("layouts/saved_room.txt", scene, room)) {
                scene.setStatus("Loaded room from layouts/saved_room.txt");
            } else {
                scene.setStatus("No saved layout found. Save first with [K].");
            }
            break;
        case 'p':
        case 'P':
            scene.loadSampleRoom();
            break;
        case 'n':
        case 'N':
            scene.clear();
            break;

        default:
            break;
    }
    glutPostRedisplay();
}

void InputHandler::handleSpecialKeys(int key, int x, int y, Scene& scene, Room& room, Camera& camera) {
    float moveStep = 0.25f;

    // If an object is selected, arrow keys move the object; otherwise they pan the camera
    if (scene.getSelected() != nullptr) {
        switch (key) {
            case GLUT_KEY_UP:
                scene.moveSelected(0.0f, -moveStep, room);
                break;
            case GLUT_KEY_DOWN:
                scene.moveSelected(0.0f, moveStep, room);
                break;
            case GLUT_KEY_LEFT:
                scene.moveSelected(-moveStep, 0.0f, room);
                break;
            case GLUT_KEY_RIGHT:
                scene.moveSelected(moveStep, 0.0f, room);
                break;
            default:
                break;
        }
    } else {
        float panStep = 0.5f;
        switch (key) {
            case GLUT_KEY_UP:
                camera.pan(0.0f, -panStep);
                break;
            case GLUT_KEY_DOWN:
                camera.pan(0.0f, panStep);
                break;
            case GLUT_KEY_LEFT:
                camera.pan(-panStep, 0.0f);
                break;
            case GLUT_KEY_RIGHT:
                camera.pan(panStep, 0.0f);
                break;
            default:
                break;
        }
    }
    glutPostRedisplay();
}

void InputHandler::handleMouse(int button, int state, int x, int y, Scene& scene, Camera& camera) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isLeftDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isLeftDragging = false;
        }
    } else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            isRightDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isRightDragging = false;
        }
    } else if (button == 3) {
        // Wheel UP -> Zoom in
        camera.zoom(-0.8f);
        glutPostRedisplay();
    } else if (button == 4) {
        // Wheel DOWN -> Zoom out
        camera.zoom(0.8f);
        glutPostRedisplay();
    }
}

void InputHandler::handleMotion(int x, int y, Camera& camera) {
    int deltaX = x - lastMouseX;
    int deltaY = y - lastMouseY;

    if (isLeftDragging) {
        camera.orbit(deltaX * 0.4f, -deltaY * 0.4f);
        glutPostRedisplay();
    } else if (isRightDragging) {
        camera.pan(-deltaX * 0.02f, -deltaY * 0.02f);
        glutPostRedisplay();
    }

    lastMouseX = x;
    lastMouseY = y;
}
