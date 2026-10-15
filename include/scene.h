#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "furniture.h"
#include "room.h"

class Scene {
public:
    std::vector<Furniture> items;
    int selectedIndex;
    int nextId;

    // Status message for user feedback (e.g., "Added Bed", "Collision Warning", "Saved Room")
    std::string statusMessage;
    float statusTimer;

    Scene();

    void clear();
    void loadSampleRoom();

    // Furniture management
    void addFurniture(FurnitureType type, const Room& room);
    void deleteSelected();
    void duplicateSelected(const Room& room);
    void selectNext();
    void selectPrev();
    void selectNone();

    // Transformations
    void moveSelected(float dx, float dz, const Room& room);
    void rotateSelected(float dAngle);
    void scaleSelected(float factor);
    void cycleSelectedColor();

    // Collision detection between bounding boxes
    bool checkCollision(int index) const;

    // Rendering
    void render() const;

    Furniture* getSelected();
    const Furniture* getSelected() const;

    void setStatus(const std::string& msg);
};

#endif // SCENE_H
