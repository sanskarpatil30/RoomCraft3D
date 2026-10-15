#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <string>

// RGBA Color structure
struct Color {
    float r, g, b, a;
    Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f)
        : r(red), g(green), b(blue), a(alpha) {}
};

// Tasteful modern interior design palette
namespace Palette {
    const Color OffWhite    (0.95f, 0.94f, 0.92f, 1.0f);  // Base wall color
    const Color WarmBeige   (0.89f, 0.85f, 0.79f, 1.0f);  // Secondary wall tone
    const Color Charcoal    (0.18f, 0.20f, 0.22f, 1.0f);  // Frame, trim, accents
    const Color SlateGray   (0.35f, 0.38f, 0.42f, 1.0f);  // Secondary accents
    const Color WarmWood    (0.62f, 0.42f, 0.28f, 1.0f);  // Teak / Oak floor & furniture
    const Color DarkWalnut  (0.32f, 0.21f, 0.14f, 1.0f);  // Dark wood accents
    const Color LightOak    (0.82f, 0.68f, 0.50f, 1.0f);  // Natural birch / oak
    const Color MutedOrange (0.86f, 0.45f, 0.24f, 1.0f);  // Terracotta accent
    const Color SageGreen   (0.48f, 0.58f, 0.48f, 1.0f);  // Modern botanical accent
    const Color SoftNavy    (0.24f, 0.32f, 0.44f, 1.0f);  // Upholstery navy
    const Color WarmGold    (0.85f, 0.70f, 0.35f, 1.0f);  // Brass / Lamp hardware
    const Color GlassTint   (0.85f, 0.92f, 0.98f, 0.45f); // Transparent window glass
    const Color SoftCream   (0.92f, 0.90f, 0.85f, 1.0f);  // Bedding & cushions
}

class Renderer {
public:
    static bool lampLightEnabled;
    static bool ceilingLightEnabled;

    // Initialize OpenGL state (depth test, lighting, blend, shade model)
    static void init();

    // Setup scene lighting (main warm ceiling light + cool fill ambient light)
    static void setupLighting(float roomWidth, float roomHeight, float roomDepth);

    // Apply material color with ambient and diffuse response
    static void setColor(const Color& color);

    // Geometric 3D Primitives with explicit surface normals
    static void drawCuboid(float width, float height, float depth, bool wireframe = false);
    static void drawCylinder(float baseRadius, float topRadius, float height, int slices = 24);
    static void drawDisk(float innerRadius, float outerRadius, int slices = 24);
    static void drawSphere(float radius, int slices = 20, int stacks = 20);
    static void drawFloorGrid(float width, float depth, float step = 1.0f);

    // 2D Overlay rendering for UI / HUD (switches to orthographic projection)
    static void begin2D(int screenWidth, int screenHeight);
    static void end2D();
    static void draw2DPanel(float x, float y, float w, float h, const Color& color);
    static void draw2DBorder(float x, float y, float w, float h, const Color& color, float lineWidth = 1.0f);
    static void drawText(float x, float y, const std::string& text, const Color& color, void* font = nullptr);
};

#endif // RENDERER_H
