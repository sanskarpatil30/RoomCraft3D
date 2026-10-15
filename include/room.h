#pragma once
#ifndef ROOM_H
#define ROOM_H

#include "renderer.h"

class Room {
public:
    float width;        // Along X-axis (e.g. 10.0m)
    float height;       // Along Y-axis (e.g. 4.0m)
    float depth;        // Along Z-axis (e.g. 10.0m)

    // Wall & floor customization
    Color floorColor;
    Color wallColor;
    Color trimColor;

    // Visibility toggles
    bool showCeiling;
    bool showFrontWall;
    bool showGrid;

    Room(float w = 10.0f, float h = 3.6f, float d = 10.0f);

    void reset();
    void render() const;

    // Boundary checks for furniture
    bool isInside(float x, float z, float halfW = 0.0f, float halfD = 0.0f) const;

private:
    void renderFloor() const;
    void renderBackWallWithDoor() const;
    void renderLeftWallWithWindow() const;
    void renderRightWall() const;
    void renderFrontWall() const;
    void renderCeiling() const;
    void renderBaseboards() const;
    void renderCeilingFixture() const;
};

#endif // ROOM_H
