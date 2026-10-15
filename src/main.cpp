#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "camera.h"
#include "room.h"
#include "scene.h"
#include "renderer.h"
#include "input.h"
#include "save_load.h"

// Application Global State
namespace App {
    Camera camera;
    Room room;
    Scene scene;

    int windowWidth = 1280;
    int windowHeight = 800;

    bool showHUD = true;
}

// Display Callback: 3D Scene + 2D Architectural Interface
void displayCallback() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply 3D Camera Projection & View Transformations
    App::camera.applyProjection();
    App::camera.applyView();

    // Setup Dynamic Scene Lighting
    Renderer::setupLighting(App::room.width, App::room.height, App::room.depth);

    // Render Room Structure (Floor, Walls, Openings, Ceiling, Baseboards, Fixture)
    App::room.render();

    // Render Furniture Items & Selection Highlights
    App::scene.render();

    // Render 2D UI / HUD Overlay
    if (App::showHUD) {
        Renderer::begin2D(App::windowWidth, App::windowHeight);

        // 1. TOP-LEFT: Main Title & Status Banner
        Renderer::draw2DPanel(15.0f, 15.0f, 440.0f, 130.0f, Color(0.10f, 0.12f, 0.14f, 0.90f));
        Renderer::draw2DBorder(15.0f, 15.0f, 440.0f, 130.0f, Palette::WarmGold, 1.5f);

        Renderer::drawText(30.0f, 38.0f, "RoomCraft 3D - Interior Designer", Palette::WarmGold, GLUT_BITMAP_HELVETICA_18);

        std::string modeStr;
        switch (App::camera.mode) {
            case CameraMode::PERSPECTIVE_ORBIT: modeStr = "3D Perspective Orbit"; break;
            case CameraMode::TOP_DOWN_ORTHO:    modeStr = "Top-Down Blueprint (2D Ortho)"; break;
            case CameraMode::FRONT_VIEW:        modeStr = "Front Elevation"; break;
            case CameraMode::SIDE_VIEW:         modeStr = "Side Elevation"; break;
        }
        Renderer::drawText(30.0f, 60.0f, "Camera: " + modeStr, Palette::OffWhite, GLUT_BITMAP_HELVETICA_12);

        // Environment Toggles Info
        std::ostringstream envOss;
        envOss << "Ceiling Light: " << (Renderer::ceilingLightEnabled ? "ON [L]" : "OFF [L]")
               << " | Grid: " << (App::room.showGrid ? "ON [G]" : "OFF [G]")
               << " | Cutaway: " << (App::room.showFrontWall ? "NO [F]" : "YES [F]");
        Renderer::drawText(30.0f, 82.0f, envOss.str(), Palette::LightOak, GLUT_BITMAP_HELVETICA_10);

        // Dynamic Status Message Bar
        Renderer::draw2DPanel(25.0f, 96.0f, 420.0f, 36.0f, Color(0.16f, 0.18f, 0.22f, 0.95f));
        Renderer::draw2DBorder(25.0f, 96.0f, 420.0f, 36.0f, Palette::SlateGray, 1.0f);
        Renderer::drawText(35.0f, 119.0f, App::scene.statusMessage, Palette::WarmGold, GLUT_BITMAP_HELVETICA_12);

        // 2. TOP-RIGHT: Selected Furniture Property Inspector
        const Furniture* sel = App::scene.getSelected();
        float panelW = 340.0f;
        float panelX = (float)App::windowWidth - panelW - 15.0f;
        Renderer::draw2DPanel(panelX, 15.0f, panelW, 190.0f, Color(0.10f, 0.12f, 0.14f, 0.90f));
        Renderer::draw2DBorder(panelX, 15.0f, panelW, 190.0f, Palette::SlateGray, 1.5f);

        Renderer::drawText(panelX + 15.0f, 38.0f, "OBJECT INSPECTOR", Palette::WarmGold, GLUT_BITMAP_HELVETICA_12);

        if (sel != nullptr) {
            Renderer::drawText(panelX + 15.0f, 60.0f, "Active: " + sel->name, Palette::OffWhite, GLUT_BITMAP_HELVETICA_12);

            std::ostringstream posOss;
            posOss << std::fixed << std::setprecision(2)
                   << "Position: (" << sel->posX << ", " << sel->posY << ", " << sel->posZ << ") m";
            Renderer::drawText(panelX + 15.0f, 82.0f, posOss.str(), Color(0.80f, 0.85f, 0.90f, 1.0f), GLUT_BITMAP_HELVETICA_10);

            std::ostringstream rotOss;
            rotOss << "Rotation: " << (int)sel->rotY << " deg [Q / E]";
            Renderer::drawText(panelX + 15.0f, 102.0f, rotOss.str(), Color(0.80f, 0.85f, 0.90f, 1.0f), GLUT_BITMAP_HELVETICA_10);

            std::ostringstream scaleOss;
            scaleOss << "Scale: " << (int)(sel->scaleX * 100.0f) << "% [ [ / ] ]";
            Renderer::drawText(panelX + 15.0f, 122.0f, scaleOss.str(), Color(0.80f, 0.85f, 0.90f, 1.0f), GLUT_BITMAP_HELVETICA_10);

            // Color Swatch Indicator
            Renderer::drawText(panelX + 15.0f, 144.0f, "Color Palette: [C] Cycle", Palette::LightOak, GLUT_BITMAP_HELVETICA_10);
            Renderer::draw2DPanel(panelX + 195.0f, 132.0f, 24.0f, 16.0f, sel->primaryColor);
            Renderer::draw2DBorder(panelX + 195.0f, 132.0f, 24.0f, 16.0f, Palette::OffWhite, 1.0f);

            // Collision Indicator
            bool collision = App::scene.checkCollision(App::scene.selectedIndex);
            if (collision) {
                Renderer::drawText(panelX + 15.0f, 172.0f, "STATUS: COLLISION OVERLAP", Color(1.0f, 0.35f, 0.35f, 1.0f), GLUT_BITMAP_HELVETICA_12);
            } else {
                Renderer::drawText(panelX + 15.0f, 172.0f, "STATUS: VALID PLACEMENT", Palette::SageGreen, GLUT_BITMAP_HELVETICA_12);
            }
        } else {
            Renderer::drawText(panelX + 15.0f, 75.0f, "No item selected.", Color(0.6f, 0.6f, 0.6f, 1.0f), GLUT_BITMAP_HELVETICA_12);
            Renderer::drawText(panelX + 15.0f, 105.0f, "Press [TAB] or [SPACE] to select", Palette::OffWhite, GLUT_BITMAP_HELVETICA_10);
            Renderer::drawText(panelX + 15.0f, 125.0f, "Press 1-6 to insert furniture", Palette::OffWhite, GLUT_BITMAP_HELVETICA_10);
        }

        // 3. BOTTOM: Comprehensive Control Panel
        float bottomBarH = 100.0f;
        float bottomBarY = (float)App::windowHeight - bottomBarH - 15.0f;
        float bottomBarW = (float)App::windowWidth - 30.0f;

        Renderer::draw2DPanel(15.0f, bottomBarY, bottomBarW, bottomBarH, Color(0.10f, 0.12f, 0.14f, 0.90f));
        Renderer::draw2DBorder(15.0f, bottomBarY, bottomBarW, bottomBarH, Palette::WarmGold, 1.0f);

        // Row 1: Add Furniture
        Renderer::drawText(30.0f, bottomBarY + 22.0f,
                           "ADD FURNITURE: [1] Bed  [2] Chair  [3] Table  [4] Sofa  [5] Wardrobe  [6] Floor Lamp",
                           Palette::WarmGold, GLUT_BITMAP_HELVETICA_12);

        // Row 2: Object Editing
        Renderer::drawText(30.0f, bottomBarY + 44.0f,
                           "EDIT OBJECT: [WASD / Arrows] Move  |  [Q/E] Rotate  |  [ [ / ] ] Scale  |  [C] Recolor  |  [U] Duplicate  |  [Del] Delete",
                           Palette::OffWhite, GLUT_BITMAP_HELVETICA_12);

        // Row 3: Navigation & System
        Renderer::drawText(30.0f, bottomBarY + 66.0f,
                           "CAMERA: Left-Drag Orbit  |  Right-Drag Pan  |  Wheel Zoom  |  [V] Cycle View (3D/2D)  |  [R] Reset Camera",
                           Color(0.80f, 0.85f, 0.90f, 1.0f), GLUT_BITMAP_HELVETICA_10);

        Renderer::drawText(30.0f, bottomBarY + 86.0f,
                           "SYSTEM: [K] Save Room  |  [O] Load Room  |  [P] Sample Preset  |  [N] Clear  |  [L] Light  |  [H] Toggle UI  |  [ESC] Exit",
                           Palette::LightOak, GLUT_BITMAP_HELVETICA_10);

        Renderer::end2D();
    }

    glutSwapBuffers();
}

// Reshape Callback
void reshapeCallback(int width, int height) {
    App::windowWidth = width;
    App::windowHeight = height;
    App::camera.updateViewport(width, height);
    glutPostRedisplay();
}

// Mouse Button Callback
void mouseButtonCallback(int button, int state, int x, int y) {
    InputHandler::handleMouse(button, state, x, y, App::scene, App::camera);
}

// Mouse Drag Motion Callback
void mouseMotionCallback(int x, int y) {
    InputHandler::handleMotion(x, y, App::camera);
}

// Keyboard Callback
void keyboardCallback(unsigned char key, int x, int y) {
    if (key == 'h' || key == 'H') {
        App::showHUD = !App::showHUD;
        glutPostRedisplay();
        return;
    }
    InputHandler::handleKeyboard(key, x, y, App::scene, App::room, App::camera);
}

// Special Keys Callback (Arrow keys)
void specialKeysCallback(int key, int x, int y) {
    InputHandler::handleSpecialKeys(key, x, y, App::scene, App::room, App::camera);
}

int main(int argc, char** argv) {
    std::cout << "========================================================\n";
    std::cout << "  RoomCraft 3D -- Interactive Interior Designer\n";
    std::cout << "  C++ OpenGL Computer Graphics Mini Project\n";
    std::cout << "========================================================\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(App::windowWidth, App::windowHeight);
    glutInitWindowPosition(80, 50);
    glutCreateWindow("RoomCraft 3D - Interactive Interior Designer");

    // Initialize OpenGL Rendering Engine
    Renderer::init();

    // Register Callbacks
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutMouseFunc(mouseButtonCallback);
    glutMotionFunc(mouseMotionCallback);
    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(specialKeysCallback);

    std::cout << "[Init] RoomCraft 3D initialized successfully.\n";
    std::cout << "[Info] Ready to run. Entering GLUT main loop.\n";

    glutMainLoop();

    return 0;
}
