#include "renderer.h"
#include <GL/freeglut.h>
#include <GL/glu.h>

bool Renderer::lampLightEnabled = true;
bool Renderer::ceilingLightEnabled = true;

void Renderer::init() {
    // 1. Depth Testing: Ensures polygons closer to the camera obscure those behind them
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // 2. Smooth Shading: Gouraud shading interpolates color/light across polygon vertices
    glShadeModel(GL_SMOOTH);

    // 3. Normalization: Automatically scales normal vectors to unit length after scaling transformations
    glEnable(GL_NORMALIZE);

    // 4. Alpha Blending: Required for transparent glass windows and translucent UI overlays
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 5. Material Color Tracking: Makes glColor calls automatically update ambient & diffuse material properties
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Specular highlight settings
    GLfloat matSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat matShininess[] = { 32.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

    // Background color: Modern slate dark grey for rich contrast
    glClearColor(0.12f, 0.13f, 0.15f, 1.0f);
}

void Renderer::setupLighting(float roomWidth, float roomHeight, float roomDepth) {
    glEnable(GL_LIGHTING);

    // LIGHT 0: Main Ceiling Pendant / Sun light (Warm White)
    if (ceilingLightEnabled) {
        glEnable(GL_LIGHT0);
        GLfloat light0Pos[]     = { 0.0f, roomHeight - 0.2f, 0.0f, 1.0f }; // Positional light at ceiling
        GLfloat light0Ambient[] = { 0.25f, 0.24f, 0.22f, 1.0f };
        GLfloat light0Diffuse[] = { 0.90f, 0.88f, 0.82f, 1.0f };
        GLfloat light0Specular[]= { 0.50f, 0.48f, 0.40f, 1.0f };

        glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);

        // Gentle quadratic attenuation for realistic indoor light falloff
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.04f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005f);
    } else {
        glDisable(GL_LIGHT0);
    }

    // LIGHT 1: Soft Ambient Fill Light (Slight cool outdoor bounce)
    glEnable(GL_LIGHT1);
    GLfloat light1Pos[]     = { roomWidth * 0.8f, roomHeight * 1.5f, roomDepth * 0.8f, 0.0f }; // Directional
    GLfloat light1Ambient[] = { 0.18f, 0.20f, 0.22f, 1.0f };
    GLfloat light1Diffuse[] = { 0.25f, 0.28f, 0.32f, 1.0f };
    GLfloat light1Specular[]= { 0.10f, 0.10f, 0.12f, 1.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1Ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1Specular);
}

void Renderer::setColor(const Color& color) {
    glColor4f(color.r, color.g, color.b, color.a);
}

void Renderer::drawCuboid(float width, float height, float depth, bool wireframe) {
    // Draws a centered cuboid extending [-w/2, w/2], [0, h], [-d/2, d/2]
    // Base is at y = 0 for intuitive ground alignment.
    float x1 = -width * 0.5f;
    float x2 =  width * 0.5f;
    float y1 =  0.0f;
    float y2 =  height;
    float z1 = -depth * 0.5f;
    float z2 =  depth * 0.5f;

    GLenum mode = wireframe ? GL_LINE_LOOP : GL_QUADS;

    // Front face (normal +Z)
    glBegin(mode);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    glEnd();

    // Back face (normal -Z)
    glBegin(mode);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x2, y1, z1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z1);
    glEnd();

    // Top face (normal +Y)
    glBegin(mode);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);
    glEnd();

    // Bottom face (normal -Y)
    glBegin(mode);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);
    glEnd();

    // Right face (normal +X)
    glBegin(mode);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glEnd();

    // Left face (normal -X)
    glBegin(mode);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y2, z1);
    glEnd();
}

void Renderer::drawCylinder(float baseRadius, float topRadius, float height, int slices) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    // GLU cylinder grows along +Z, rotate -90 around X to stand upright along +Y
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, baseRadius, topRadius, height, slices, 4);

    // Draw bottom cap
    glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0, baseRadius, slices, 1);
    glPopMatrix();

    // Draw top cap
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, height);
    gluDisk(quad, 0.0, topRadius, slices, 1);
    glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(quad);
}

void Renderer::drawDisk(float innerRadius, float outerRadius, int slices) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, innerRadius, outerRadius, slices, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

void Renderer::drawSphere(float radius, int slices, int stacks) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluSphere(quad, radius, slices, stacks);
    gluDeleteQuadric(quad);
}

void Renderer::drawFloorGrid(float width, float depth, float step) {
    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);
    glColor4f(0.25f, 0.27f, 0.30f, 0.5f);

    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;

    glBegin(GL_LINES);
    for (float x = -halfW; x <= halfW + 0.001f; x += step) {
        glVertex3f(x, 0.005f, -halfD);
        glVertex3f(x, 0.005f,  halfD);
    }
    for (float z = -halfD; z <= halfD + 0.001f; z += step) {
        glVertex3f(-halfW, 0.005f, z);
        glVertex3f( halfW, 0.005f, z);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

// 2D Overlay Rendering for HUD / Menu
void Renderer::begin2D(int screenWidth, int screenHeight) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, screenWidth, screenHeight, 0.0); // (0,0) at top-left

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void Renderer::end2D() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void Renderer::draw2DPanel(float x, float y, float w, float h, const Color& color) {
    setColor(color);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void Renderer::draw2DBorder(float x, float y, float w, float h, const Color& color, float lineWidth) {
    setColor(color);
    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void Renderer::drawText(float x, float y, const std::string& text, const Color& color, void* font) {
    setColor(color);
    glRasterPos2f(x, y);
    void* glutFont = (font != nullptr) ? font : GLUT_BITMAP_HELVETICA_12;
    for (char c : text) {
        glutBitmapCharacter(glutFont, c);
    }
}
