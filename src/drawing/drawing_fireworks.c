/**
 * @file drawing_fireworks.c
 * @brief Fireworks effect implementation
 */

#include "drawing/drawing_fireworks.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define GRAVITY 0.3f
#define PARTICLE_SPEED 5.0f
#define PARTICLE_LIFE_DECAY 0.02f

static COLORREF fireworkColors[] = {
    RGB(255, 0, 0),    // red
    RGB(0, 255, 0),    // green
    RGB(0, 0, 255),    // blue
    RGB(255, 255, 0),  // yellow
    RGB(255, 0, 255),  // magenta
    RGB(0, 255, 255),  // cyan
    RGB(255, 165, 0),  // orange
    RGB(128, 0, 128),  // purple
};

#define NUM_COLORS (sizeof(fireworkColors) / sizeof(fireworkColors[0]))

void InitFireworks(FireworksEffect* fw, int windowWidth, int windowHeight) {
    srand((unsigned int)time(NULL));
    fw->particleCount = 0;
    fw->active = FALSE;
    fw->startTime = 0;
    fw->windowWidth = windowWidth;
    fw->windowHeight = windowHeight;
}

void StartFireworks(FireworksEffect* fw, int windowWidth, int windowHeight) {
    fw->windowWidth = windowWidth;
    fw->windowHeight = windowHeight;
    fw->startTime = GetTickCount();
    fw->active = TRUE;
    fw->particleCount = 0;

    // Create multiple bursts
    int numBursts = 3 + rand() % 3;  // 3-5 bursts
    for (int burst = 0; burst < numBursts; burst++) {
        // Random center position, biased towards center
        float centerX = windowWidth * 0.3f + (rand() % (int)(windowWidth * 0.4f));
        float centerY = windowHeight * 0.3f + (rand() % (int)(windowHeight * 0.4f));

        int particlesPerBurst = 20 + rand() % 30;  // 20-50 particles per burst
        COLORREF burstColor = fireworkColors[rand() % NUM_COLORS];

        for (int i = 0; i < particlesPerBurst && fw->particleCount < MAX_FIREWORK_PARTICLES; i++) {
            FireworkParticle* p = &fw->particles[fw->particleCount++];
            p->x = centerX;
            p->y = centerY;

            // Random direction
            float angle = (rand() % 360) * 3.14159f / 180.0f;
            float speed = PARTICLE_SPEED * (0.5f + (rand() % 100) / 100.0f);

            p->vx = cosf(angle) * speed;
            p->vy = sinf(angle) * speed;
            p->life = 1.0f;
            p->color = burstColor;
        }
    }
}

void UpdateFireworks(FireworksEffect* fw, DWORD currentTime) {
    if (!fw->active) return;

    if (currentTime - fw->startTime > FIREWORK_DURATION_MS) {
        fw->active = FALSE;
        fw->particleCount = 0;
        return;
    }

    for (int i = 0; i < fw->particleCount; i++) {
        FireworkParticle* p = &fw->particles[i];
        if (p->life <= 0) continue;

        p->x += p->vx;
        p->y += p->vy;
        p->vy += GRAVITY;  // gravity
        p->life -= PARTICLE_LIFE_DECAY;

        // Remove dead particles
        if (p->life <= 0) {
            // Move last particle to this position
            if (i < fw->particleCount - 1) {
                fw->particles[i] = fw->particles[--fw->particleCount];
                i--;  // recheck this index
            } else {
                fw->particleCount--;
            }
        }
    }
}

void DrawFireworks(HDC hdc, FireworksEffect* fw) {
    if (!fw->active) return;

    for (int i = 0; i < fw->particleCount; i++) {
        FireworkParticle* p = &fw->particles[i];
        if (p->life <= 0) continue;

        // Particle size based on window size, scaled
        int size = (int)(fw->windowWidth * 0.01f * p->life * 2 + 1);
        if (size < 1) size = 1;

        // Create brush with alpha (simulate transparency)
        HBRUSH brush = CreateSolidBrush(p->color);
        SelectObject(hdc, brush);

        // Draw particle as small circle
        Ellipse(hdc, (int)p->x - size, (int)p->y - size, (int)p->x + size, (int)p->y + size);

        DeleteObject(brush);
    }
}

BOOL IsFireworksActive(FireworksEffect* fw) {
    return fw->active;
}