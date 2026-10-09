#include "common.h"
typedef struct { float position[3], rotation[3], distance, angle; } View;
typedef struct { s32 elapsed, duration; } Transition;
extern View D_801658B4, D_801658D4;
extern Transition D_801658F4[8];
extern void func_80059728(View *);
/* Starts camera transition channel 0-7: records the current view value as the
 * start, the scaled request as the target, and skips the transition when the
 * two are equal. */
void func_8005B0A8(u32 channel, s32 duration, float value) {
    View current;
    Transition *transition = &D_801658F4[channel];
    float start = 0.0f, target = 0.0f;
    func_80059728(&current);
    transition->elapsed = 0;
    transition->duration = duration;
    switch (channel) {
    case 0:
        start = D_801658B4.position[0] = current.position[0];
        target = D_801658D4.position[0] = value / 10.0f; break;
    case 1:
        start = D_801658B4.position[1] = current.position[1];
        target = D_801658D4.position[1] = value / 10.0f; break;
    case 2:
        start = D_801658B4.position[2] = current.position[2];
        target = D_801658D4.position[2] = value / 10.0f; break;
    case 3:
        start = D_801658B4.rotation[0] = current.rotation[0];
        target = D_801658D4.rotation[0] = value / 1000.0f; break;
    case 4:
        start = D_801658B4.rotation[1] = current.rotation[1];
        target = D_801658D4.rotation[1] = value / 1000.0f; break;
    case 5:
        start = D_801658B4.rotation[2] = current.rotation[2];
        target = D_801658D4.rotation[2] = value / 1000.0f; break;
    case 6:
        start = D_801658B4.distance = current.distance;
        target = D_801658D4.distance = value / 10.0f; break;
    case 7:
        target = D_801658D4.angle = value;
        start = D_801658B4.angle = current.angle; break;
    }
    if (target == start) transition->duration = 0;
}
