#include "room.h"
#include <GL/freeglut.h>
#include <GL/glu.h>

Room::Room(float w, float h, float d)
    : width(w), height(h), depth(d),
      floorColor(Palette::WarmWood),
      wallColor(Palette::OffWhite),
      trimColor(Palette::Charcoal),
      showCeiling(false),
      showFrontWall(false),
      showGrid(true) {
}

void Room::reset() {
    width = 10.0f;
    height = 3.6f;
    depth = 10.0f;
    floorColor = Palette::WarmWood;
    wallColor = Palette::OffWhite;
    trimColor = Palette::Charcoal;
    showCeiling = false;
    showFrontWall = false;
    showGrid = true;
}

bool Room::isInside(float x, float z, float halfW, float halfD) const {
    float limitX = (width * 0.5f) - 0.2f - halfW;
    float limitZ = (depth * 0.5f) - 0.2f - halfD;
    return (x >= -limitX && x <= limitX && z >= -limitZ && z <= limitZ);
}

void Room::render() const {
    renderFloor();
    renderBackWallWithDoor();
    renderLeftWallWithWindow();
    renderRightWall();
    if (showFrontWall) renderFrontWall();
    if (showCeiling) renderCeiling();
    renderBaseboards();
    renderCeilingFixture();

    if (showGrid) {
        Renderer::drawFloorGrid(width, depth, 1.0f);
    }
}

void Room::renderFloor() const {
    // Parquet wooden plank pattern using alternating subtle color shifts
    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;
    int planksX = 10;
    int planksZ = 10;
    float stepX = width / planksX;
    float stepZ = depth / planksZ;

    for (int i = 0; i < planksX; ++i) {
        for (int j = 0; j < planksZ; ++j) {
            float x1 = -halfW + i * stepX;
            float x2 = x1 + stepX;
            float z1 = -halfD + j * stepZ;
            float z2 = z1 + stepZ;

            // Subtle checker/parquet wood grain variation
            float tone = ((i + j) % 2 == 0) ? 1.0f : 0.94f;
            Color tileColor(floorColor.r * tone, floorColor.g * tone, floorColor.b * tone, 1.0f);
            Renderer::setColor(tileColor);

            glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f); // Facing UP
            glVertex3f(x1, 0.0f, z2);
            glVertex3f(x2, 0.0f, z2);
            glVertex3f(x2, 0.0f, z1);
            glVertex3f(x1, 0.0f, z1);
            glEnd();
        }
    }
}

void Room::renderBackWallWithDoor() const {
    // Back wall is at z = -depth / 2, normal faces +Z into the room
    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;
    float zBack = -halfD;

    // Doorway opening parameters
    float doorW = 1.4f;
    float doorH = 2.4f;
    float doorCenter = 1.5f; // Offset along X
    float doorLeft = doorCenter - doorW * 0.5f;
    float doorRight = doorCenter + doorW * 0.5f;

    Renderer::setColor(wallColor);

    // Left wall segment (from left edge to door left)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-halfW, 0.0f, zBack);
    glVertex3f(doorLeft, 0.0f, zBack);
    glVertex3f(doorLeft, height, zBack);
    glVertex3f(-halfW, height, zBack);
    glEnd();

    // Right wall segment (from door right to right edge)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(doorRight, 0.0f, zBack);
    glVertex3f(halfW, 0.0f, zBack);
    glVertex3f(halfW, height, zBack);
    glVertex3f(doorRight, height, zBack);
    glEnd();

    // Header above the door
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(doorLeft, doorH, zBack);
    glVertex3f(doorRight, doorH, zBack);
    glVertex3f(doorRight, height, zBack);
    glVertex3f(doorLeft, height, zBack);
    glEnd();

    // Door Frame (Charcoal trim)
    Renderer::setColor(trimColor);
    float frameThickness = 0.08f;
    // Left jamb
    glPushMatrix();
    glTranslatef(doorLeft - frameThickness * 0.5f, 0.0f, zBack + 0.02f);
    Renderer::drawCuboid(frameThickness, doorH + frameThickness, 0.08f);
    glPopMatrix();
    // Right jamb
    glPushMatrix();
    glTranslatef(doorRight + frameThickness * 0.5f, 0.0f, zBack + 0.02f);
    Renderer::drawCuboid(frameThickness, doorH + frameThickness, 0.08f);
    glPopMatrix();
    // Top head jamb
    glPushMatrix();
    glTranslatef(doorCenter, doorH, zBack + 0.02f);
    Renderer::drawCuboid(doorW + frameThickness * 2.0f, frameThickness, 0.08f);
    glPopMatrix();

    // Interior Wood Door Panel (slightly ajar / recessed)
    glPushMatrix();
    glTranslatef(doorCenter, 0.0f, zBack - 0.05f);
    Renderer::setColor(Palette::DarkWalnut);
    Renderer::drawCuboid(doorW - 0.04f, doorH - 0.02f, 0.06f);

    // Modern Gold Handle
    glTranslatef(doorW * 0.35f, 1.05f, 0.06f);
    Renderer::setColor(Palette::WarmGold);
    Renderer::drawSphere(0.04f, 12, 12);
    glPopMatrix();
}

void Room::renderLeftWallWithWindow() const {
    // Left wall is at x = -width / 2, normal faces +X into the room
    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;
    float xLeft = -halfW;

    // Window cutout parameters
    float winW = 2.6f;
    float winH = 1.8f;
    float winSillY = 1.0f;
    float winCenterZ = 0.0f;
    float winZ1 = winCenterZ - winW * 0.5f;
    float winZ2 = winCenterZ + winW * 0.5f;

    Renderer::setColor(wallColor);

    // Left segment (back part of left wall)
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(xLeft, 0.0f, -halfD);
    glVertex3f(xLeft, 0.0f, winZ1);
    glVertex3f(xLeft, height, winZ1);
    glVertex3f(xLeft, height, -halfD);
    glEnd();

    // Right segment (front part of left wall)
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(xLeft, 0.0f, winZ2);
    glVertex3f(xLeft, 0.0f, halfD);
    glVertex3f(xLeft, height, halfD);
    glVertex3f(xLeft, height, winZ2);
    glEnd();

    // Bottom wall under window sill
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(xLeft, 0.0f, winZ1);
    glVertex3f(xLeft, 0.0f, winZ2);
    glVertex3f(xLeft, winSillY, winZ2);
    glVertex3f(xLeft, winSillY, winZ1);
    glEnd();

    // Top wall above window lintel
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(xLeft, winSillY + winH, winZ1);
    glVertex3f(xLeft, winSillY + winH, winZ2);
    glVertex3f(xLeft, height, winZ2);
    glVertex3f(xLeft, height, winZ1);
    glEnd();

    // Window Frame & Sill (Charcoal)
    Renderer::setColor(trimColor);
    // Sill
    glPushMatrix();
    glTranslatef(xLeft + 0.06f, winSillY - 0.04f, winCenterZ);
    Renderer::drawCuboid(0.18f, 0.06f, winW + 0.16f);
    glPopMatrix();

    // Window Frame Borders
    glPushMatrix();
    glTranslatef(xLeft + 0.03f, winSillY, winZ1);
    Renderer::drawCuboid(0.08f, winH, 0.06f);
    glTranslatef(0.0f, 0.0f, winW);
    Renderer::drawCuboid(0.08f, winH, 0.06f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xLeft + 0.03f, winSillY + winH - 0.04f, winCenterZ);
    Renderer::drawCuboid(0.08f, 0.06f, winW);
    glPopMatrix();

    // Window Mullions (Crossbars)
    glPushMatrix();
    glTranslatef(xLeft + 0.02f, winSillY + winH * 0.5f, winCenterZ);
    Renderer::drawCuboid(0.04f, 0.04f, winW); // Horizontal bar
    glTranslatef(0.0f, -winH * 0.5f, 0.0f);
    Renderer::drawCuboid(0.04f, winH, 0.04f); // Vertical bar
    glPopMatrix();

    // Transparent Glass Pane
    glDepthMask(GL_FALSE); // Don't write to depth buffer for transparent surface
    Renderer::setColor(Palette::GlassTint);
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(xLeft + 0.01f, winSillY, winZ1);
    glVertex3f(xLeft + 0.01f, winSillY, winZ2);
    glVertex3f(xLeft + 0.01f, winSillY + winH, winZ2);
    glVertex3f(xLeft + 0.01f, winSillY + winH, winZ1);
    glEnd();
    glDepthMask(GL_TRUE); // Re-enable depth writes
}

void Room::renderRightWall() const {
    // Right wall at x = +width / 2, normal faces -X
    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;

    Renderer::setColor(wallColor);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(halfW, 0.0f, halfD);
    glVertex3f(halfW, 0.0f, -halfD);
    glVertex3f(halfW, height, -halfD);
    glVertex3f(halfW, height, halfD);
    glEnd();

    // Decorative Modern Art Canvas on right wall
    glPushMatrix();
    glTranslatef(halfW - 0.03f, 1.6f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

    // Frame
    Renderer::setColor(Palette::Charcoal);
    Renderer::drawCuboid(2.2f, 1.4f, 0.04f);

    // Canvas artwork with warm terracotta accent
    glTranslatef(0.0f, 0.05f, 0.03f);
    Renderer::setColor(Palette::MutedOrange);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.95f, 0.0f, 0.0f);
    glVertex3f( 0.95f, 0.0f, 0.0f);
    glVertex3f( 0.95f, 1.25f, 0.0f);
    glVertex3f(-0.95f, 1.25f, 0.0f);
    glEnd();

    // Geometric art elements inside canvas
    Renderer::setColor(Palette::WarmGold);
    glPushMatrix();
    glTranslatef(0.0f, 0.65f, 0.01f);
    Renderer::drawDisk(0.0f, 0.35f, 32);
    glPopMatrix();

    glPopMatrix();
}

void Room::renderFrontWall() const {
    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;

    Renderer::setColor(wallColor);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(halfW, 0.0f, halfD);
    glVertex3f(-halfW, 0.0f, halfD);
    glVertex3f(-halfW, height, halfD);
    glVertex3f(halfW, height, halfD);
    glEnd();
}

void Room::renderCeiling() const {
    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;

    Renderer::setColor(Palette::OffWhite);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f); // Facing DOWN
    glVertex3f(-halfW, height, -halfD);
    glVertex3f(halfW, height, -halfD);
    glVertex3f(halfW, height, halfD);
    glVertex3f(-halfW, height, halfD);
    glEnd();
}

void Room::renderBaseboards() const {
    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;
    float baseH = 0.12f;
    float baseThick = 0.03f;

    Renderer::setColor(trimColor);

    // Left baseboard
    glPushMatrix();
    glTranslatef(-halfW + baseThick * 0.5f, 0.0f, 0.0f);
    Renderer::drawCuboid(baseThick, baseH, depth);
    glPopMatrix();

    // Right baseboard
    glPushMatrix();
    glTranslatef(halfW - baseThick * 0.5f, 0.0f, 0.0f);
    Renderer::drawCuboid(baseThick, baseH, depth);
    glPopMatrix();

    // Back baseboard (excluding door)
    // Left section of back wall baseboard
    float doorW = 1.4f;
    float doorCenter = 1.5f;
    float doorLeft = doorCenter - doorW * 0.5f;
    float doorRight = doorCenter + doorW * 0.5f;

    float len1 = (doorLeft - (-halfW));
    glPushMatrix();
    glTranslatef(-halfW + len1 * 0.5f, 0.0f, -halfD + baseThick * 0.5f);
    Renderer::drawCuboid(len1, baseH, baseThick);
    glPopMatrix();

    float len2 = (halfW - doorRight);
    glPushMatrix();
    glTranslatef(doorRight + len2 * 0.5f, 0.0f, -halfD + baseThick * 0.5f);
    Renderer::drawCuboid(len2, baseH, baseThick);
    glPopMatrix();
}

void Room::renderCeilingFixture() const {
    // Elegant hanging pendant light in center of room
    glPushMatrix();
    glTranslatef(0.0f, height, 0.0f);

    // Ceiling mount plate
    Renderer::setColor(Palette::Charcoal);
    Renderer::drawCylinder(0.25f, 0.25f, 0.04f, 20);

    // Suspension cord / rod
    glTranslatef(0.0f, -0.6f, 0.0f);
    Renderer::setColor(Palette::Charcoal);
    Renderer::drawCylinder(0.02f, 0.02f, 0.6f, 12);

    // Lamp shade (inverted cone/bell)
    glTranslatef(0.0f, -0.2f, 0.0f);
    Renderer::setColor(Palette::WarmGold);
    Renderer::drawCylinder(0.40f, 0.15f, 0.25f, 24);

    // Glowing bulb
    if (Renderer::ceilingLightEnabled) {
        Renderer::setColor(Color(1.0f, 0.95f, 0.8f, 1.0f));
    } else {
        Renderer::setColor(Color(0.5f, 0.5f, 0.5f, 1.0f));
    }
    glTranslatef(0.0f, 0.05f, 0.0f);
    Renderer::drawSphere(0.10f, 16, 16);

    glPopMatrix();
}
