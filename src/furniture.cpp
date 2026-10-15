#include "furniture.h"
#include <cmath>
#include <GL/freeglut.h>
#include <GL/glu.h>

Furniture::Furniture(FurnitureType t, float x, float z, int itemId)
    : id(itemId), type(t), posX(x), posY(0.0f), posZ(z),
      rotY(0.0f), scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f),
      isSelected(false) {

    switch (type) {
        case FurnitureType::BED:
            name = "King Bed";
            primaryColor = Palette::SoftNavy;
            secondaryColor = Palette::WarmWood;
            break;
        case FurnitureType::CHAIR:
            name = "Dining Chair";
            primaryColor = Palette::MutedOrange;
            secondaryColor = Palette::DarkWalnut;
            break;
        case FurnitureType::TABLE:
            name = "Dining Table";
            primaryColor = Palette::WarmWood;
            secondaryColor = Palette::Charcoal;
            break;
        case FurnitureType::SOFA:
            name = "Lounge Sofa";
            primaryColor = Palette::SageGreen;
            secondaryColor = Palette::WarmWood;
            break;
        case FurnitureType::WARDROBE:
            name = "Wardrobe";
            primaryColor = Palette::DarkWalnut;
            secondaryColor = Palette::WarmGold;
            break;
        case FurnitureType::FLOOR_LAMP:
            name = "Floor Lamp";
            primaryColor = Palette::SoftCream;
            secondaryColor = Palette::WarmGold;
            break;
    }
}

void Furniture::getExtents(float& halfW, float& halfD, float& height) const {
    switch (type) {
        case FurnitureType::BED:
            halfW = 1.1f * scaleX;
            halfD = 1.2f * scaleZ;
            height = 1.1f * scaleY;
            break;
        case FurnitureType::CHAIR:
            halfW = 0.35f * scaleX;
            halfD = 0.35f * scaleZ;
            height = 0.95f * scaleY;
            break;
        case FurnitureType::TABLE:
            halfW = 1.1f * scaleX;
            halfD = 0.7f * scaleZ;
            height = 0.85f * scaleY;
            break;
        case FurnitureType::SOFA:
            halfW = 1.25f * scaleX;
            halfD = 0.55f * scaleZ;
            height = 0.85f * scaleY;
            break;
        case FurnitureType::WARDROBE:
            halfW = 0.85f * scaleX;
            halfD = 0.40f * scaleZ;
            height = 2.1f * scaleY;
            break;
        case FurnitureType::FLOOR_LAMP:
            halfW = 0.3f * scaleX;
            halfD = 0.3f * scaleZ;
            height = 1.75f * scaleY;
            break;
    }
}

void Furniture::cycleColor() {
    // Array of elegant interior fabric & wood tones
    static const Color colors[] = {
        Palette::SoftNavy,
        Palette::MutedOrange,
        Palette::SageGreen,
        Palette::WarmWood,
        Palette::DarkWalnut,
        Palette::Charcoal,
        Palette::WarmBeige,
        Palette::SoftCream
    };
    static const int numColors = sizeof(colors) / sizeof(colors[0]);

    for (int i = 0; i < numColors; ++i) {
        if (std::fabs(primaryColor.r - colors[i].r) < 0.05f &&
            std::fabs(primaryColor.g - colors[i].g) < 0.05f &&
            std::fabs(primaryColor.b - colors[i].b) < 0.05f) {
            primaryColor = colors[(i + 1) % numColors];
            return;
        }
    }
    primaryColor = colors[0];
}

void Furniture::renderBoundingBox() const {
    float halfW, halfD, height;
    getExtents(halfW, halfD, height);

    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glColor4f(1.0f, 0.84f, 0.0f, 0.9f); // Vibrant gold highlight outline

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // Draw wireframe cuboid
    Renderer::drawCuboid(halfW * 2.0f + 0.1f, height + 0.05f, halfD * 2.0f + 0.1f, true);

    // Subtle corner indicators
    float cx = halfW + 0.05f;
    float cz = halfD + 0.05f;
    float ch = height + 0.05f;

    glBegin(GL_LINES);
    // Draw vertical edge markers
    glVertex3f(-cx, 0.0f, -cz); glVertex3f(-cx, ch, -cz);
    glVertex3f( cx, 0.0f, -cz); glVertex3f( cx, ch, -cz);
    glVertex3f( cx, 0.0f,  cz); glVertex3f( cx, ch,  cz);
    glVertex3f(-cx, 0.0f,  cz); glVertex3f(-cx, ch,  cz);
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void Furniture::render() const {
    glPushMatrix();

    // 1. Translation: Places object in the room coordinate space
    glTranslatef(posX, posY, posZ);

    // 2. Rotation: Rotates around vertical Y-axis (facing direction)
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // 3. Scaling: Resizes object along X, Y, Z dimensions
    glScalef(scaleX, scaleY, scaleZ);

    switch (type) {
        case FurnitureType::BED:        renderBed(); break;
        case FurnitureType::CHAIR:      renderChair(); break;
        case FurnitureType::TABLE:      renderTable(); break;
        case FurnitureType::SOFA:       renderSofa(); break;
        case FurnitureType::WARDROBE:   renderWardrobe(); break;
        case FurnitureType::FLOOR_LAMP: renderFloorLamp(); break;
    }

    glPopMatrix();

    // Render selection highlight box if currently active
    if (isSelected) {
        renderBoundingBox();
    }
}

// -------------------------------------------------------------
// 1. BED RENDERING
// -------------------------------------------------------------
void Furniture::renderBed() const {
    float bedW = 2.0f;
    float bedL = 2.2f;

    // Wooden base frame
    Renderer::setColor(secondaryColor);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    Renderer::drawCuboid(bedW, 0.25f, bedL);
    glPopMatrix();

    // 4 Corner Wooden Legs
    for (float dx : { -0.92f, 0.92f }) {
        for (float dz : { -1.02f, 1.02f }) {
            glPushMatrix();
            glTranslatef(dx, 0.0f, dz);
            Renderer::drawCuboid(0.12f, 0.30f, 0.12f);
            glPopMatrix();
        }
    }

    // Tall Headboard at back (-Z)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -bedL * 0.5f + 0.06f);
    Renderer::drawCuboid(bedW + 0.1f, 1.1f, 0.12f);

    // Upholstered headboard insert
    glTranslatef(0.0f, 0.25f, 0.07f);
    Renderer::setColor(primaryColor);
    Renderer::drawCuboid(bedW - 0.15f, 0.75f, 0.04f);
    glPopMatrix();

    // Mattress (Soft Cream)
    Renderer::setColor(Palette::SoftCream);
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, 0.05f);
    Renderer::drawCuboid(bedW - 0.1f, 0.32f, bedL - 0.18f);
    glPopMatrix();

    // Duvet / Quilt covering lower 65% of mattress
    Renderer::setColor(primaryColor);
    glPushMatrix();
    glTranslatef(0.0f, 0.32f, 0.28f);
    Renderer::drawCuboid(bedW - 0.06f, 0.28f, bedL * 0.65f);

    // Folded edge of duvet
    glTranslatef(0.0f, 0.28f, -bedL * 0.32f);
    Renderer::setColor(Palette::WarmGold);
    Renderer::drawCuboid(bedW - 0.05f, 0.06f, 0.25f);
    glPopMatrix();

    // 2 Fluffy Pillows
    Renderer::setColor(Palette::OffWhite);
    for (float px : { -0.45f, 0.45f }) {
        glPushMatrix();
        glTranslatef(px, 0.58f, -0.65f);
        glRotatef(12.0f, 1.0f, 0.0f, 0.0f); // Tilted against headboard
        Renderer::drawCuboid(0.65f, 0.14f, 0.42f);
        glPopMatrix();
    }
}

// -------------------------------------------------------------
// 2. CHAIR RENDERING
// -------------------------------------------------------------
void Furniture::renderChair() const {
    float seatW = 0.55f;
    float seatD = 0.55f;
    float seatH = 0.48f;

    // 4 Wooden legs
    Renderer::setColor(secondaryColor);
    for (float dx : { -0.22f, 0.22f }) {
        for (float dz : { -0.22f, 0.22f }) {
            glPushMatrix();
            glTranslatef(dx, 0.0f, dz);
            Renderer::drawCylinder(0.025f, 0.018f, seatH, 16);
            glPopMatrix();
        }
    }

    // Seat cushion
    Renderer::setColor(primaryColor);
    glPushMatrix();
    glTranslatef(0.0f, seatH, 0.0f);
    Renderer::drawCuboid(seatW, 0.08f, seatD);
    glPopMatrix();

    // Backrest posts
    Renderer::setColor(secondaryColor);
    for (float bx : { -0.22f, 0.22f }) {
        glPushMatrix();
        glTranslatef(bx, seatH + 0.08f, -0.22f);
        Renderer::drawCylinder(0.02f, 0.02f, 0.45f, 12);
        glPopMatrix();
    }

    // Backrest top rail & ergonomic cushion
    glPushMatrix();
    glTranslatef(0.0f, seatH + 0.30f, -0.22f);
    Renderer::setColor(primaryColor);
    Renderer::drawCuboid(seatW - 0.05f, 0.22f, 0.05f);
    glPopMatrix();
}

// -------------------------------------------------------------
// 3. TABLE RENDERING
// -------------------------------------------------------------
void Furniture::renderTable() const {
    float topW = 2.0f;
    float topD = 1.1f;
    float tableH = 0.78f;

    // 4 Solid legs
    Renderer::setColor(secondaryColor);
    for (float lx : { -topW * 0.5f + 0.12f, topW * 0.5f - 0.12f }) {
        for (float lz : { -topD * 0.5f + 0.12f, topD * 0.5f - 0.12f }) {
            glPushMatrix();
            glTranslatef(lx, 0.0f, lz);
            Renderer::drawCuboid(0.08f, tableH, 0.08f);
            glPopMatrix();
        }
    }

    // Apron support frame under tabletop
    glPushMatrix();
    glTranslatef(0.0f, tableH - 0.08f, 0.0f);
    Renderer::drawCuboid(topW - 0.16f, 0.06f, topD - 0.16f);
    glPopMatrix();

    // Tabletop
    Renderer::setColor(primaryColor);
    glPushMatrix();
    glTranslatef(0.0f, tableH, 0.0f);
    Renderer::drawCuboid(topW, 0.07f, topD);

    // Decorative table runner / centerpiece
    glTranslatef(0.0f, 0.075f, 0.0f);
    Renderer::setColor(Palette::WarmGold);
    Renderer::drawDisk(0.0f, 0.16f, 24);
    glPopMatrix();
}

// -------------------------------------------------------------
// 4. SOFA RENDERING
// -------------------------------------------------------------
void Furniture::renderSofa() const {
    float sofaW = 2.3f;
    float sofaD = 0.95f;

    // 4 Angled Peg Feet
    Renderer::setColor(secondaryColor);
    for (float fx : { -sofaW * 0.5f + 0.15f, sofaW * 0.5f - 0.15f }) {
        for (float fz : { -sofaD * 0.5f + 0.15f, sofaD * 0.5f - 0.15f }) {
            glPushMatrix();
            glTranslatef(fx, 0.0f, fz);
            Renderer::drawCylinder(0.035f, 0.02f, 0.14f, 12);
            glPopMatrix();
        }
    }

    // Base Support Plinth
    glPushMatrix();
    glTranslatef(0.0f, 0.14f, 0.0f);
    Renderer::setColor(secondaryColor);
    Renderer::drawCuboid(sofaW, 0.08f, sofaD);
    glPopMatrix();

    // 2 Main Plush Seat Cushions
    float cushionW = (sofaW - 0.44f) * 0.5f;
    Renderer::setColor(primaryColor);
    for (float cx : { -cushionW * 0.52f, cushionW * 0.52f }) {
        glPushMatrix();
        glTranslatef(cx, 0.22f, 0.05f);
        Renderer::drawCuboid(cushionW - 0.03f, 0.22f, sofaD - 0.25f);
        glPopMatrix();
    }

    // Backrest
    glPushMatrix();
    glTranslatef(0.0f, 0.22f, -sofaD * 0.5f + 0.12f);
    Renderer::drawCuboid(sofaW, 0.62f, 0.24f);
    glPopMatrix();

    // Left and Right Armrests
    for (float ax : { -sofaW * 0.5f + 0.11f, sofaW * 0.5f - 0.11f }) {
        glPushMatrix();
        glTranslatef(ax, 0.22f, 0.02f);
        Renderer::drawCuboid(0.22f, 0.44f, sofaD - 0.08f);
        glPopMatrix();
    }

    // 2 Throw Pillows in Accent Terracotta / Gold
    Renderer::setColor(Palette::MutedOrange);
    for (float px : { -sofaW * 0.5f + 0.32f, sofaW * 0.5f - 0.32f }) {
        glPushMatrix();
        glTranslatef(px, 0.44f, -0.15f);
        glRotatef(px < 0 ? 25.0f : -25.0f, 0.0f, 1.0f, 0.0f);
        Renderer::drawCuboid(0.32f, 0.32f, 0.10f);
        glPopMatrix();
    }
}

// -------------------------------------------------------------
// 5. WARDROBE RENDERING
// -------------------------------------------------------------
void Furniture::renderWardrobe() const {
    float w = 1.6f;
    float h = 2.1f;
    float d = 0.70f;

    // Main cabinet body
    Renderer::setColor(primaryColor);
    glPushMatrix();
    glTranslatef(0.0f, 0.08f, 0.0f);
    Renderer::drawCuboid(w, h, d);
    glPopMatrix();

    // Bottom plinth / kick
    Renderer::setColor(Palette::Charcoal);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    Renderer::drawCuboid(w + 0.04f, 0.08f, d + 0.04f);
    glPopMatrix();

    // Top crown molding
    glPushMatrix();
    glTranslatef(0.0f, h + 0.08f, 0.0f);
    Renderer::drawCuboid(w + 0.06f, 0.06f, d + 0.06f);
    glPopMatrix();

    // Center divider groove line between double doors
    Renderer::setColor(Palette::Charcoal);
    glPushMatrix();
    glTranslatef(0.0f, 0.12f, d * 0.5f + 0.01f);
    Renderer::drawCuboid(0.02f, h - 0.08f, 0.02f);
    glPopMatrix();

    // Sleek vertical brass handles
    Renderer::setColor(secondaryColor);
    for (float hx : { -0.06f, 0.06f }) {
        glPushMatrix();
        glTranslatef(hx, 1.05f, d * 0.5f + 0.03f);
        Renderer::drawCylinder(0.012f, 0.012f, 0.28f, 12);
        glPopMatrix();
    }
}

// -------------------------------------------------------------
// 6. FLOOR LAMP RENDERING
// -------------------------------------------------------------
void Furniture::renderFloorLamp() const {
    // Weighted brass disk base
    Renderer::setColor(secondaryColor);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    Renderer::drawCylinder(0.26f, 0.24f, 0.04f, 24);
    glPopMatrix();

    // Slender brass pole
    glPushMatrix();
    glTranslatef(0.0f, 0.04f, 0.0f);
    Renderer::drawCylinder(0.025f, 0.022f, 1.45f, 16);
    glPopMatrix();

    // Lamp shade (translucent cream linen)
    glPushMatrix();
    glTranslatef(0.0f, 1.42f, 0.0f);
    Renderer::setColor(primaryColor);
    Renderer::drawCylinder(0.28f, 0.20f, 0.35f, 24);

    // Glowing warm bulb inside
    glTranslatef(0.0f, 0.12f, 0.0f);
    Renderer::setColor(Color(1.0f, 0.95f, 0.70f, 1.0f));
    Renderer::drawSphere(0.07f, 16, 16);
    glPopMatrix();
}
