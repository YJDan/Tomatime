/**
 * @file drawing_fireworks.h
 * @brief Fireworks effect for timer completion
 */

#ifndef DRAWING_FIREWORKS_H
#define DRAWING_FIREWORKS_H

#include <windows.h>

#define MAX_FIREWORK_PARTICLES 200
#define FIREWORK_DURATION_MS 3000  // 3 seconds

typedef struct {
    float x, y;          // position
    float vx, vy;        // velocity
    float life;          // 0-1, 1 is full life
    COLORREF color;
} FireworkParticle;

typedef struct {
    FireworkParticle particles[MAX_FIREWORK_PARTICLES];
    int particleCount;
    DWORD startTime;
    BOOL active;
    int windowWidth, windowHeight;  // for scaling
} FireworksEffect;

void InitFireworks(FireworksEffect* fw, int windowWidth, int windowHeight);
void StartFireworks(FireworksEffect* fw, int windowWidth, int windowHeight);
void UpdateFireworks(FireworksEffect* fw, DWORD currentTime);
void DrawFireworks(HDC hdc, FireworksEffect* fw);
BOOL IsFireworksActive(FireworksEffect* fw);

// Global fireworks state shared across rendering and timer event code
extern FireworksEffect g_fireworks;

#endif // DRAWING_FIREWORKS_H