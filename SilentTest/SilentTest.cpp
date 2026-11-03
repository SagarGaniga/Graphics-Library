#include <graphics.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Screen Dimensions
const int SCREEN_W = 800;
const int SCREEN_H = 600;
const float PI = 3.1415926535f;

// 3D Point Structure
struct Point3D {
    float x, y, z;
};

// 2D Projected Point Structure
struct Point2D {
    int x, y;
};

// Starfield Particle Structure
struct Star {
    float x, y, speed;
};

// 3D Cube Vertices (Outer Frame)
Point3D cubeVerts[8] = {
    {-70, -70, -70}, { 70, -70, -70}, { 70,  70, -70}, {-70,  70, -70},
    {-70, -70,  70}, { 70, -70,  70}, { 70,  70,  70}, {-70,  70,  70}
};

// 3D Cube Edges (12 connections)
int cubeEdges[12][2] = {
    {0,1}, {1,2}, {2,3}, {3,0}, // Back face
    {4,5}, {5,6}, {6,7}, {7,4}, // Front face
    {0,4}, {1,5}, {2,6}, {3,7}  // Connecting pillars
};

// 3D Core Octahedron Vertices (Inner Boss Core)
Point3D coreVerts[6] = {
    {  0, -55,   0}, // Top peak
    {  0,  55,   0}, // Bottom peak
    {-55,   0,   0}, // Left
    { 55,   0,   0}, // Right
    {  0,   0, -55}, // Back
    {  0,   0,  55}  // Front
};

int coreEdges[12][2] = {
    {0,2}, {0,3}, {0,4}, {0,5}, // Top connections
    {1,2}, {1,3}, {1,4}, {1,5}, // Bottom connections
    {2,4}, {4,3}, {3,5}, {5,2}  // Mid perimeter
};

// Transform and Project 3D Point to 2D Screen
Point2D projectPoint(Point3D p, float angX, float angY, float angZ, int centerX, int centerY) {
    // Rotation around X axis
    float y1 = p.y * cos(angX) - p.z * sin(angX);
    float z1 = p.y * sin(angX) + p.z * cos(angX);
    float x1 = p.x;

    // Rotation around Y axis
    float x2 = x1 * cos(angY) + z1 * sin(angY);
    float z2 = -x1 * sin(angY) + z1 * cos(angY);
    float y2 = y1;

    // Rotation around Z axis
    float x3 = x2 * cos(angZ) - y2 * sin(angZ);
    float y3 = x2 * sin(angZ) + y2 * cos(angZ);
    float z3 = z2;

    // Perspective Projection Formula
    float fov = 350.0f;
    float distance = 250.0f;
    float factor = fov / (z3 + distance);

    Point2D out;
    out.x = centerX + (int)(x3 * factor);
    out.y = centerY + (int)(y3 * factor);
    return out;
}

int main() {
    // Initialize Graphic Window with double-buffering support
    initwindow(SCREEN_W, SCREEN_H, "DEV-C++ 3D ENGINE SHOWCASE");

    // Initialize 60 Background Stars
    const int STAR_COUNT = 60;
    Star stars[STAR_COUNT];
    srand((unsigned int)time(NULL));
    for (int i = 0; i < STAR_COUNT; i++) {
        stars[i].x = (float)(rand() % SCREEN_W);
        stars[i].y = (float)(rand() % 280);
        stars[i].speed = 0.5f + (float)(rand() % 15) / 10.0f;
    }

    // Animation Variables
    float angX = 0.0f, angY = 0.0f, angZ = 0.0f;
    float gridScroll = 0.0f;
    int page = 0;
    int hudPulse = 0;

    // Main 60-FPS Render Loop
    while (!kbhit()) {
        // Switch between active and visual page (prevents any screen flickering)
        setactivepage(page);
        setvisualpage(1 - page);
        cleardevice();

        // -------------------------------------------------------------
        // 1. RENDER RETRO STARS
        // -------------------------------------------------------------
        setcolor(WHITE);
        for (int i = 0; i < STAR_COUNT; i++) {
            putpixel((int)stars[i].x, (int)stars[i].y, (i % 2 == 0) ? WHITE : LIGHTCYAN);
            stars[i].x -= stars[i].speed;
            if (stars[i].x < 0) {
                stars[i].x = SCREEN_W;
                stars[i].y = (float)(rand() % 280);
            }
        }

        // -------------------------------------------------------------
        // 2. RENDER PERSPECTIVE SYNTHWAVE GROUND GRID
        // -------------------------------------------------------------
        int horizonY = 320;
        setcolor(MAGENTA);

        // Horizon Line
        setlinestyle(SOLID_LINE, 0, 2);
        line(0, horizonY, SCREEN_W, horizonY);
        setlinestyle(SOLID_LINE, 0, 1);

        // Perspective Rays radiating from center horizon
        for (int x = -400; x <= SCREEN_W + 400; x += 80) {
            line(SCREEN_W / 2, horizonY, x, SCREEN_H);
        }

        // Animated Depth Lines moving forward
        gridScroll += 0.8f;
        if (gridScroll > 20.0f) gridScroll = 0.0f;

        for (float d = gridScroll; d < 280.0f; d += 20.0f) {
            float expY = horizonY + (d * d) / 280.0f;
            if (expY <= SCREEN_H) {
                line(0, (int)expY, SCREEN_W, (int)expY);
            }
        }

        // -------------------------------------------------------------
        // 3. RENDER 3D ROTATING BOSS ENTITY (CENTER)
        // -------------------------------------------------------------
        int coreX = SCREEN_W / 2;
        int coreY = 250;

        // Draw Outer Cube (Cyan Neon)
        Point2D p2dCube[8];
        for (int i = 0; i < 8; i++) {
            p2dCube[i] = projectPoint(cubeVerts[i], angX, angY, angZ, coreX, coreY);
        }

        setcolor(CYAN);
        setlinestyle(SOLID_LINE, 0, 2);
        for (int i = 0; i < 12; i++) {
            line(p2dCube[cubeEdges[i][0]].x, p2dCube[cubeEdges[i][0]].y,
                 p2dCube[cubeEdges[i][1]].x, p2dCube[cubeEdges[i][1]].y);
        }

        // Draw Cube Corner Glow Spheres
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        for (int i = 0; i < 8; i++) {
            fillellipse(p2dCube[i].x, p2dCube[i].y, 3, 3);
        }

        // Draw Inner Core Octahedron (Counter-rotating in Red/Yellow)
        Point2D p2dCore[6];
        for (int i = 0; i < 6; i++) {
            p2dCore[i] = projectPoint(coreVerts[i], -angX * 1.5f, -angY * 1.5f, angZ * 1.5f, coreX, coreY);
        }

        setcolor(LIGHTRED);
        setlinestyle(SOLID_LINE, 0, 2);
        for (int i = 0; i < 12; i++) {
            line(p2dCore[coreEdges[i][0]].x, p2dCore[coreEdges[i][0]].y,
                 p2dCore[coreEdges[i][1]].x, p2dCore[coreEdges[i][1]].y);
        }

        // Core Central Heart
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, (hudPulse % 10 < 5) ? LIGHTRED : YELLOW);
        fillellipse(coreX, coreY, 6, 6);

        // -------------------------------------------------------------
        // 4. SCI-FI HUD & SYSTEM TEST STATUS
        // -------------------------------------------------------------
        // Top Diagnostic Banner
        setcolor(DARKGRAY);
        rectangle(10, 10, SCREEN_W - 10, 75);
        setcolor(GREEN);
        rectangle(12, 12, SCREEN_W - 12, 73);

        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        setcolor(LIGHTGREEN);
        outtextxy(70, 20, (char*)"GRAPHICS TEST: 100% OPERATIONAL");

        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        setcolor(WHITE);
        outtextxy(72, 50, (char*)"DEV-C++ 5.11 BGI WINBGIM INITIALIZED SUCCESSFULLY");

        // Boss Core Power Bar
        setcolor(LIGHTCYAN);
        outtextxy(30, 490, (char*)"CORE ENGINE POWER:");
        setcolor(WHITE);
        rectangle(180, 488, 620, 504);
        setfillstyle(SOLID_FILL, LIGHTGREEN);
        bar(182, 490, 618, 502);

        outtextxy(630, 490, (char*)"100% OK");

        // Interactive Footer Instruction
        setcolor((hudPulse % 20 < 10) ? YELLOW : WHITE);
        outtextxy(260, 560, (char*)"PRESS ANY KEY ON KEYBOARD TO EXIT");

        // Increment Rotation & Pulse Timers
        angX += 0.025f;
        angY += 0.035f;
        angZ += 0.015f;
        hudPulse++;

        // Flip Buffer
        page = 1 - page;
        delay(16); // ~60 FPS Target
    }

    // Exit cleanly
    closegraph();
    return 0;
}
