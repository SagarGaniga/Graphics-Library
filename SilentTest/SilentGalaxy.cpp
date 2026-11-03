#include <graphics.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>

const int W = 960, H = 600;
const float PI = 3.14159265f;

// Primitives
void fillCircle(int x, int y, int r, int color) {
    if (r <= 0) return;
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y, r, r);
}

void fillRect(int x1, int y1, int x2, int y2, int color) {
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    bar(x1, y1, x2, y2);
}

// Building profile
struct Building { int x, w, h; };
Building city[9] = {
    {40, 75, 240}, {125, 95, 320}, {230, 65, 210}, {305, 100, 370},
    {415, 120, 280}, {545, 85, 350}, {640, 100, 230}, {750, 85, 310}, {845, 80, 250}
};

int main() {
    initwindow(W, H, "Cosmic Odyssey: Abyss to Deep Space", 50, 30);

    int page = 0;
    int state = 0; // 0: Solar, 1: Cloud Dive, 2: Cityscape, 3: Deep Abyss, 4: Cosmic Ascent
    int timer = 0;

    // Astronomy trackers
    float earthOrb = 0.0f, moonOrb = 0.0f;
    float meteorX = 940.0f, meteorY = 40.0f;
    bool meteorActive = true;

    // City & Ocean trackers
    float dayCycle = 0.0f;
    float waveOffset = 0.0f;

    while (!kbhit()) {
        setactivepage(page);
        cleardevice();

        // =============================================================
        // PHASE 0: SOLAR SYSTEM & METEOR STORM
        // =============================================================
        if (state == 0) {
            fillRect(0, 0, W, H, COLOR(2, 4, 10));

            // Starfield with twinkle
            srand(42);
            for (int i = 0; i < 110; i++) {
                int sx = rand() % W, sy = rand() % H;
                int b = 140 + (int)(110 * sin(timer * 0.08f + i));
                putpixel(sx, sy, COLOR(b, b, b));
            }

            int sunX = 180, sunY = 300;

            // Corona and Sun
            fillCircle(sunX, sunY, 60, COLOR(255, 90, 0));
            fillCircle(sunX, sunY, 48, COLOR(255, 175, 20));
            fillCircle(sunX, sunY, 38, COLOR(255, 245, 160));

            // Orbits
            setcolor(COLOR(35, 42, 60));
            ellipse(sunX, sunY, 0, 360, 130, 65);  // Venus
            ellipse(sunX, sunY, 0, 360, 300, 150); // Earth
            ellipse(sunX, sunY, 0, 360, 460, 230); // Mars
            ellipse(sunX, sunY, 0, 360, 610, 305); // Saturn

            // Venus
            int vx = sunX + (int)(130 * cos(earthOrb * 1.35f));
            int vy = sunY + (int)(65 * sin(earthOrb * 1.35f));
            fillCircle(vx, vy, 8, COLOR(225, 175, 105));

            // Mars
            int marX = sunX + (int)(460 * cos(earthOrb * 0.65f - 1.2f));
            int marY = sunY + (int)(230 * sin(earthOrb * 0.65f - 1.2f));
            fillCircle(marX, marY, 10, COLOR(220, 85, 45));

            // Saturn with tilt rings
            int satX = sunX + (int)(610 * cos(earthOrb * 0.4f + 2.0f));
            int satY = sunY + (int)(305 * sin(earthOrb * 0.4f + 2.0f));
            setcolor(COLOR(190, 165, 120));
            ellipse(satX, satY, 0, 360, 26, 7);
            fillCircle(satX, satY, 13, COLOR(215, 180, 125));

            // Earth & Moon
            int eX = sunX + (int)(300 * cos(earthOrb));
            int eY = sunY + (int)(150 * sin(earthOrb));
            fillCircle(eX, eY, 21, COLOR(30, 115, 245));
            fillCircle(eX - 4, eY - 3, 9, COLOR(55, 195, 95));
            fillCircle(eX + 6, eY + 4, 6, COLOR(55, 195, 95));

            int mX = eX + (int)(44 * cos(moonOrb));
            int mY = eY + (int)(22 * sin(moonOrb));
            fillCircle(mX, mY, 5, COLOR(215, 220, 230));

            // Blazing Meteor with dynamic spark tail
            if (timer > 40 && meteorActive) {
                for (int t = 0; t < 30; t++) {
                    int tx = (int)meteorX + t * 5;
                    int ty = (int)meteorY - t * 2 + ((t % 3) - 1) * 2;
                    int flameR = 255;
                    int flameG = (255 - t * 8 > 0) ? 255 - t * 8 : 0;
                    int flameB = (180 - t * 10 > 0) ? 180 - t * 10 : 0;
                    putpixel(tx, ty, COLOR(flameR, flameG, flameB));
                    if (t % 2 == 0) putpixel(tx, ty + 1, COLOR(flameR, flameG, flameB));
                }
                fillCircle((int)meteorX, (int)meteorY, 4, COLOR(255, 255, 255));
                meteorX -= 15.0f;
                meteorY += 6.5f;
                if (meteorX < -50) meteorActive = false;
            }

            // HUD
            setcolor(COLOR(0, 230, 200));
            outtextxy(30, 30, (char*)"[SYSTEM: HELIOCENTRIC SCAN -> LOCKING TARGET: EARTH]");

            earthOrb += 0.016f;
            moonOrb += 0.065f;
            timer++;
            if (timer > 180) { state = 1; timer = 0; }
        }

        // =============================================================
        // PHASE 1: HYPER-SPEED CLOUD BREAKTHROUGH
        // =============================================================
        else if (state == 1) {
            float progress = (float)timer / 50.0f;
            int r = (int)(25 + progress * 620);
            fillRect(0, 0, W, H, COLOR(2, 4, 10));

            // Atmosphere Glow expansion
            fillCircle(W / 2, H / 2, r + 50, COLOR(15, 55, 130));
            fillCircle(W / 2, H / 2, r + 15, COLOR(60, 160, 255));
            fillCircle(W / 2, H / 2, r, COLOR(25, 105, 225));

            // Volumetric clouds parting outward
            int shift = (int)(progress * 460);
            for (int i = 0; i < 8; i++) {
                int cy = 80 + i * 65;
                fillCircle(240 - shift + (i * 20), cy, 60, COLOR(235, 242, 255));
                fillCircle(720 + shift - (i * 20), cy, 60, COLOR(235, 242, 255));
            }

            setcolor(COLOR(255, 255, 255));
            outtextxy(30, 30, (char*)"[ENTRY: STRATOSPHERE CLOUDS PARTING...]");

            timer++;
            if (timer >= 50) { state = 2; timer = 0; dayCycle = 0.0f; }
        }

        // =============================================================
        // PHASE 2: METROPOLIS DAY & NIGHT CYCLE
        // =============================================================
        else if (state == 2) {
            float sunHeight = sin(dayCycle);

            // Sky gradient calculation
            int skyR = (sunHeight > 0.0f) ? (int)(15 + (1.0f - sunHeight) * 190) : 6;
            int skyG = (sunHeight > 0.0f) ? (int)(50 + sunHeight * 140) : 10;
            int skyB = (sunHeight > 0.0f) ? (int)(90 + sunHeight * 155) : 28;
            fillRect(0, 0, W, 440, COLOR(skyR, skyG, skyB));

            // Night Stars
            if (sunHeight < 0.2f) {
                srand(99);
                for (int i = 0; i < 50; i++) {
                    int sx = rand() % W, sy = rand() % 230;
                    putpixel(sx, sy, COLOR(225, 225, 255));
                }
            }

            // Celestial bodies
            int bodyX, bodyY;
            if (sunHeight >= 0.0f) {
                bodyX = (int)(W / 2 - 400 * cos(dayCycle));
                bodyY = (int)(410 - 320 * sin(dayCycle));
                fillCircle(bodyX, bodyY, 28, COLOR(255, 160, 30));
                fillCircle(bodyX, bodyY, 22, COLOR(255, 235, 90));
            } else {
                bodyX = (int)(W / 2 - 400 * cos(dayCycle + PI));
                bodyY = (int)(410 - 320 * sin(dayCycle + PI));
                fillCircle(bodyX, bodyY, 22, COLOR(190, 215, 245));
                fillCircle(bodyX, bodyY, 18, COLOR(240, 245, 255));
                fillCircle(bodyX + 8, bodyY - 3, 14, COLOR(skyR, skyG, skyB)); // Crescent
            }

            // Skyscrapers
            for (int i = 0; i < 9; i++) {
                int bx = city[i].x, bw = city[i].w, bh = city[i].h;
                int topY = 440 - bh;
                int bCol = (sunHeight > 0.0f) ? COLOR(35, 42, 55) : COLOR(14, 18, 28);
                fillRect(bx, topY, bx + bw, 440, bCol);

                // Beacon antenna
                setcolor(COLOR(130, 130, 140));
                line(bx + bw / 2, topY, bx + bw / 2, topY - 24);
                if ((timer / 15) % 2 == 0) fillCircle(bx + bw / 2, topY - 24, 2, COLOR(255, 40, 40));

                // Illuminated Windows
                for (int wy = topY + 16; wy < 430; wy += 18) {
                    for (int wx = bx + 8; wx < bx + bw - 8; wx += 14) {
                        if (sunHeight < 0.25f && ((wx * 13 + wy * 7) % 5 != 0)) {
                            int winCol = ((wx + wy) % 2 == 0) ? COLOR(255, 220, 100) : COLOR(100, 220, 255);
                            fillRect(wx, wy, wx + 6, wy + 10, winCol);
                        } else if (sunHeight >= 0.25f) {
                            fillRect(wx, wy, wx + 6, wy + 10, COLOR(70, 95, 125));
                        }
                    }
                }
            }

            // Highway & Traffic with dynamic beam headlights
            fillRect(0, 435, W, 445, COLOR(20, 22, 28));
            for (int c = 0; c < 5; c++) {
                // Eastbound (Headlights)
                int c1X = (int)(timer * 7 + c * 200) % (W + 60) - 30;
                fillRect(c1X, 437, c1X + 18, 443, COLOR(200, 40, 40));
                fillCircle(c1X + 18, 440, 2, COLOR(255, 255, 210));
                setcolor(COLOR(255, 255, 180));
                line(c1X + 18, 440, c1X + 45, 440);

                // Westbound (Taillights)
                int c2X = W - ((int)(timer * 8 + c * 220) % (W + 60));
                fillRect(c2X, 441, c2X + 18, 445, COLOR(30, 100, 210));
                fillCircle(c2X + 18, 443, 2, COLOR(255, 30, 30));
            }

            // Ocean Surface
            int seaCol = (sunHeight > 0.0f) ? COLOR(16, 68, 128) : COLOR(5, 20, 48);
            fillRect(0, 445, W, H, seaCol);

            // Water light reflection shimmer
            int refCol = (sunHeight > 0.0f) ? COLOR(255, 200, 80) : COLOR(170, 210, 255);
            for (int ry = 448; ry < H; ry += 7) {
                int jitter = (rand() % 30) - 15;
                int span = 8 + (ry - 445) / 5;
                setcolor(refCol);
                line(bodyX + jitter - span, ry, bodyX + jitter + span, ry);
            }

            // Multi-layered waves
            for (int l = 0; l < 3; l++) {
                int baseY = 465 + l * 42;
                int waveCol = (sunHeight > 0.0f) ? COLOR(40 + l * 20, 110 + l * 25, 180 + l * 20)
                                                 : COLOR(15 + l * 15, 45 + l * 15, 95 + l * 25);
                setcolor(waveCol);
                for (int x = 0; x < W - 6; x += 6) {
                    int y1 = baseY + (int)(5 * sin(x * 0.025f + waveOffset + l * 1.5f));
                    int y2 = baseY + (int)(5 * sin((x + 6) * 0.025f + waveOffset + l * 1.5f));
                    line(x, y1, x + 6, y2);
                }
            }

            setcolor(COLOR(0, 240, 210));
            if (sunHeight > 0.0f) outtextxy(30, 30, (char*)"[SURFACE: PACIFIC METROPOLIS - SOLAR CYCLE]");
            else outtextxy(30, 30, (char*)"[SURFACE: PACIFIC METROPOLIS - MIDNIGHT HORIZON]");

            dayCycle += 0.016f;
            waveOffset += 0.08f;
            timer++;

            if (dayCycle >= 2 * PI) { state = 3; timer = 0; }
        }

        // =============================================================
        // PHASE 3: DEEP SEA ABYSS (JELLYFISH & MARINE LIFE)
        // =============================================================
        else if (state == 3) {
            // Abyssal deep navy background
            fillRect(0, 0, W, H, COLOR(3, 14, 38));

            // God Rays filtering down
            for (int i = 0; i < 6; i++) {
                int gx = 80 + i * 160 + (int)(25 * sin(timer * 0.03f + i));
                setcolor(COLOR(10, 40, 80));
                line(gx, 0, gx + 80, H);
            }

            // School of darting fish
            for (int i = 0; i < 5; i++) {
                int fx = (timer * 6 + i * 130) % (W + 90) - 40;
                int fy = 160 + i * 75 + (int)(15 * sin(timer * 0.1f + i));
                fillCircle(fx, fy, 8, COLOR(255, 140, 40));
                // Fish tail
                line(fx - 8, fy, fx - 16, fy - 5);
                line(fx - 8, fy, fx - 16, fy + 5);
                line(fx - 16, fy - 5, fx - 16, fy + 5);
            }

            // Bioluminescent pulsing Jellyfish
            int jx = W / 2 + (int)(60 * sin(timer * 0.035f));
            int jy = 330 + (int)(35 * cos(timer * 0.045f));
            int pulse = 28 + (int)(6 * sin(timer * 0.12f));

            fillCircle(jx, jy, pulse, COLOR(150, 60, 255));
            fillCircle(jx, jy + 3, pulse - 6, COLOR(210, 140, 255));

            // Dynamic wavy tentacles
            for (int t = -16; t <= 16; t += 8) {
                int tx = jx + t;
                int tw = (int)(10 * sin(timer * 0.15f + t));
                setcolor(COLOR(100, 240, 255));
                line(tx, jy + pulse - 4, tx + tw, jy + pulse + 45);
                line(tx + tw, jy + pulse + 45, tx, jy + pulse + 85);
            }

            // Ascending bubbles
            for (int b = 0; b < 20; b++) {
                int bx = (b * 49 + 23) % W + (int)(8 * sin(timer * 0.09f + b));
                int by = H - ((timer * 4 + b * 37) % H);
                setcolor(COLOR(160, 230, 255));
                circle(bx, by, 3 + (b % 4));
            }

            setcolor(COLOR(100, 240, 255));
            outtextxy(30, 30, (char*)"[SUB-SURFACE: MARIANA ABYSS - BIOLUMINESCENT FAUNA]");

            timer++;
            if (timer > 200) { state = 4; timer = 0; }
        }

        // =============================================================
        // PHASE 4: ASCENT TO INFINITE COSMOS (SEAMLESS REBOOT)
        // =============================================================
        else if (state == 4) {
            float progress = (float)timer / 45.0f;
            int r = (int)(600 - progress * 580);
            fillRect(0, 0, W, H, COLOR(2, 4, 10));

            // Ascending planetary overview
            fillCircle(W / 2, H / 2, r + 35, COLOR(20, 75, 175));
            fillCircle(W / 2, H / 2, r, COLOR(35, 125, 240));

            setcolor(COLOR(0, 230, 200));
            outtextxy(30, 30, (char*)"[ORBIT ASCENT: ESCAPING GRAVITY -> RETURNING TO COSMOS]");

            timer++;
            if (timer >= 45) {
                state = 0;
                timer = 0;
                meteorActive = true;
                meteorX = 940.0f;
                meteorY = 40.0f;
            }
        }

        // Double Buffering Swap
        setvisualpage(page);
        page = 1 - page;
        delay(22);
    }

    closegraph();
    return 0;
}
