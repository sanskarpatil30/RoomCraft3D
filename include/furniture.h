#pragma once
#ifndef FURNITURE_H
#define FURNITURE_H

#include <string>
#include "renderer.h"

enum class FurnitureType {
    BED,
    CHAIR,
    TABLE,
    SOFA,
    WARDROBE,
    FLOOR_LAMP
};

class Furniture {
public:
    int id;
    FurnitureType type;
    std::string name;

    // Transform properties
    float posX, posY, posZ;
    float rotY;                 // Rotation around vertical Y-axis in degrees
    float scaleX, scaleY, scaleZ;

    // Colors
    Color primaryColor;
    Color secondaryColor;

    // Selection flag
    bool isSelected;

    Furniture(FurnitureType t, float x = 0.0f, float z = 0.0f, int itemId = 1);

    void render() const;
    void renderBoundingBox() const;

    // Bounding extent for boundary and collision checks
    void getExtents(float& halfW, float& halfD, float& height) const;

    // Cycle through aesthetic interior color presets
    void cycleColor();

private:
    void renderBed() const;
    void renderChair() const;
    void renderTable() const;
    void renderSofa() const;
    void renderWardrobe() const;
    void renderFloorLamp() const;
};

#endif // FURNITURE_H
