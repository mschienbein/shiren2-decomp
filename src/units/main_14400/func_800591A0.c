#include "common.h"

typedef unsigned short u16;
typedef float f32;
typedef struct { f32 x, y, z; } Vec3;
/* Camera view (same layout as func_80058EF0's view): eye, scale, depth, range. */
typedef struct { Vec3 eye; Vec3 scale; f32 depth; f32 range; } View;
typedef struct { u16 scale, depth, range; } FloorPreset;
typedef struct { u16 height, scale_x, scale_y, depth; } TowerPreset;

extern View D_8013B120, D_80165364;
extern FloorPreset D_8013B174[];
extern TowerPreset D_8013B21C[];
extern s32 D_80165384;
s32 func_800627D4(void);
s32 func_800627C4(void);

/* Resets the camera view to its defaults, then applies the preset for the
 * current dungeon type (func_800627C4) and floor (func_800627D4). */
void func_800591A0(void) {
    u16 floor = func_800627D4();
    short tower = floor;
    Vec3 *scale;

    D_80165384 = 0;
    D_80165364.eye = D_8013B120.eye;
    D_80165364.scale = D_8013B120.scale;
    D_80165364.depth = D_8013B120.depth;
    D_80165364.range = D_8013B120.range;
    scale = &D_80165364.scale;
    switch (func_800627C4()) {
    case 1: {
        FloorPreset *preset = &D_8013B174[(short)floor];
        s32 depth, range;

        D_80165364.scale.x = preset->scale / 1000.0f;
        depth = preset->depth;
        range = preset->range;
        D_80165364.depth = depth;
        D_80165364.range = range;
        break;
    }
    case 4: {
        TowerPreset *preset;

        if ((short)floor >= 0x11) tower = 0;
        preset = &D_8013B21C[tower];
        D_80165364.eye.y = preset->height;
        D_80165364.scale.x = preset->scale_x / 1000.0f;
        scale->y = preset->scale_y / 1000.0f;
        D_80165364.depth = preset->depth;
        D_80165364.range = 41.0f;
        break;
    }
    }
}
