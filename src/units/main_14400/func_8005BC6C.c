#include "common.h"
typedef struct { float x, y, z; } Vector;
typedef struct { float position[3], rotation[3], distance, angle; } View;
typedef struct { u32 elapsed, duration; } Transition;
extern View D_801658B4, D_801658D4;
extern Transition D_801658F4[8];
extern Vector D_80165300, D_80165324;
extern float D_8016533C, D_80165340;
extern s32 D_80165404;
extern float func_8005C3C8(float, float, float), func_8005C3E4(float, float, float);
extern float func_8005C3FC(float, float, float), func_8005C454(float, float, float);
extern void func_80059668(float);
void func_8005BC6C(void) {
    s32 channel;
    Transition *transition;
    for (channel = 0, transition = D_801658F4; channel < 8; channel++, transition++) {
        float *destination, *start, *end, phase;
        if (transition->duration == 0) continue;
        switch (channel) {
        case 0: default: destination = &D_80165300.x; start = &D_801658B4.position[0]; end = &D_801658D4.position[0]; break;
        case 1: destination = &D_80165300.y; start = &D_801658B4.position[1]; end = &D_801658D4.position[1]; break;
        case 2: destination = &D_80165300.z; start = &D_801658B4.position[2]; end = &D_801658D4.position[2]; break;
        case 3: destination = &D_80165324.x; start = &D_801658B4.rotation[0]; end = &D_801658D4.rotation[0]; break;
        case 4: destination = &D_80165324.y; start = &D_801658B4.rotation[1]; end = &D_801658D4.rotation[1]; break;
        case 5: destination = &D_80165324.z; start = &D_801658B4.rotation[2]; end = &D_801658D4.rotation[2]; break;
        case 6: destination = &D_8016533C; start = &D_801658B4.distance; end = &D_801658D4.distance; break;
        case 7: destination = &D_80165340; start = &D_801658B4.angle; end = &D_801658D4.angle; break;
        }
        transition->elapsed++;
        if (transition->elapsed >= transition->duration) {
            *destination = *end;
            transition->duration = 0;
            continue;
        }
        phase = (float)transition->elapsed / (float)transition->duration;
        if ((u32)(channel - 3) < 3) {
            float delta = *end - *start;
            delta += delta > 3.141592654 ? -6.283185308 :
                (delta <= -3.141592654 ? 6.283185308 : 0.0);
            switch (D_80165404) {
            case 0: *destination = func_8005C3E4(*start, delta, phase); break;
            case 1: *destination = func_8005C454(*start, delta, phase); break;
            }
        } else {
            switch (D_80165404) {
            case 0: *destination = func_8005C3C8(*start, *end, phase); break;
            case 1: *destination = func_8005C3FC(*start, *end, phase); break;
            }
        }
    }
    func_80059668(D_80165340);
}
