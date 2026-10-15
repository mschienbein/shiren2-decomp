#include "common.h"
typedef struct { float x, y, z; } Vector;
typedef struct { Vector position, offset, rotation; float distance, angle; } CameraKey;
extern Vector D_80165300, D_8016530C, D_80165324;
extern float D_8016533C, D_80165340;
extern void func_80059668(float);
extern float func_8005C3C8(float, float, float), func_8005C3E4(float, float, float);
extern float func_8005C3FC(float, float, float), func_8005C454(float, float, float);
void func_8005BF7C(CameraKey *start, CameraKey *end, u32 elapsed, u32 duration, s32 mode, s32 offset) {
    Vector *destination, *from, *to;
    if (!offset) { destination = &D_80165300; from = &start->position; to = &end->position; }
    else { destination = &D_8016530C; from = &start->offset; to = &end->offset; }
    if (!elapsed) {
        *destination = *from;
        D_80165324 = start->rotation;
        D_8016533C = start->distance; D_80165340 = start->angle;
    } else if (elapsed == duration) {
        *destination = *to;
        D_80165324 = end->rotation;
        D_8016533C = end->distance; D_80165340 = end->angle;
    } else {
        float phase = (float)elapsed / (float)duration;
        float dx = end->rotation.x - start->rotation.x;
        float dy = end->rotation.y - start->rotation.y;
        float dz = end->rotation.z - start->rotation.z;
        dx += dx > 3.141592654 ? -6.283185308 :
            (dx <= -3.141592654 ? 6.283185308 : 0.0);
        dy += dy > 3.141592654 ? -6.283185308 :
            (dy <= -3.141592654 ? 6.283185308 : 0.0);
        dz += dz > 3.141592654 ? -6.283185308 :
            (dz <= -3.141592654 ? 6.283185308 : 0.0);
        switch (mode) {
        case 0:
            destination->x = func_8005C3C8(from->x, to->x, phase);
            destination->y = func_8005C3C8(from->y, to->y, phase);
            destination->z = func_8005C3C8(from->z, to->z, phase);
            D_80165324.x = func_8005C3E4(start->rotation.x, dx, phase);
            D_80165324.y = func_8005C3E4(start->rotation.y, dy, phase);
            D_80165324.z = func_8005C3E4(start->rotation.z, dz, phase);
            D_8016533C = func_8005C3C8(start->distance, end->distance, phase);
            D_80165340 = func_8005C3C8(start->angle, end->angle, phase);
            break;
        case 1:
            destination->x = func_8005C3FC(from->x, to->x, phase);
            destination->y = func_8005C3FC(from->y, to->y, phase);
            destination->z = func_8005C3FC(from->z, to->z, phase);
            D_80165324.x = func_8005C454(start->rotation.x, dx, phase);
            D_80165324.y = func_8005C454(start->rotation.y, dy, phase);
            D_80165324.z = func_8005C454(start->rotation.z, dz, phase);
            D_8016533C = func_8005C3FC(start->distance, end->distance, phase);
            D_80165340 = func_8005C3FC(start->angle, end->angle, phase);
            break;
        }
    }
    func_80059668(D_80165340);
}
