#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef float f32;

typedef struct { f32 x; f32 y; f32 z; } Vec;
/* 0xB0-byte unit record of the D_801DEAB4 table */
typedef struct {
    u8 pad0[0x2];
    s16 id;
    u8 pad4[0x4];
    u8 state;
    u8 pad9[0x3];
    s16 x;
    u8 padE[0x2];
    s16 z;
    u8 pad12[0x47 - 0x12];
    u8 alpha;
    u8 pad48[0xB0 - 0x48];
} Unit;

extern s32 D_8013D8F0;
extern s32 D_8013D8FC;
extern s32 D_801E4E70;
extern Unit D_801DEAB4[];

s32 func_800627C4(void);
void func_80061AB8(f32 *arg0, f32 *arg1, f32 *minX, f32 *minZ, f32 *maxX, f32 *maxZ);
void func_800593F8(Vec *out);
s32 func_80074114(void);
s32 func_80041574(s32 a, s32 b);
s32 func_800421A8(void);
s32 func_8005CB7C(void);

void func_800775F0(void) {
    Vec pos;
    f32 minX;
    f32 minZ;
    f32 maxX;
    f32 maxZ;
    s32 mode;
    s32 cx;
    s32 cz;
    s32 self;
    s32 home;
    s32 i;
    Unit *u;
    f32 dx, dz, m;

    mode = func_800627C4();
    func_80061AB8(0, 0, &minX, &minZ, &maxX, &maxZ);
    func_800593F8(&pos);
    cx = (s32)pos.x >> 5;
    cz = (s32)pos.z >> 5;
    self = func_80074114();
    home = func_80041574(D_801DEAB4[self].x >> 7, D_801DEAB4[self].z >> 7);

    for (i = 0; i < 30; i++) {
        u = &D_801DEAB4[i];
        if (u->id == -1 || u->state != 0) {
            continue;
        }
        if (D_8013D8F0 == 1 || D_801E4E70 == 0 || D_8013D8FC == 1) {
            u->alpha = 0xFF;
        } else {
            dx = u->x * 0.25f;
            dz = u->z * 0.25f;
            if (dx < minX - 64.0f || maxX + 64.0f < dx || dz < minZ - 64.0f || maxZ + 64.0f < dz) {
                u->alpha = 0;
            } else {
                dx = (dx < minX) ? minX - dx : ((maxX < dx) ? dx - maxX : 0.0f);
                dz = (dz < minZ) ? minZ - dz : ((maxZ < dz) ? dz - maxZ : 0.0f);
                m = (__builtin_sqrtf(dx * dx + dz * dz) - 48.0f) * 15.9375f;
                if (m < 0.0f) {
                    u->alpha = 0xFF;
                } else if (m >= 255.0f) {
                    u->alpha = 0;
                } else {
                    u->alpha = 255.0f - m;
                }
            }
        }
        if (mode == 1) {
            if (func_800421A8() != 0) {
                u->alpha = 0xFF;
            } else if (home == func_80041574(u->x >> 7, u->z >> 7)) {
                u->alpha = 0xFF;
            } else {
                u->alpha = 0;
            }
        }
        if (func_8005CB7C() == 1 && self != i) {
            u->alpha = 0;
        }
    }

    for (i = 0; i < 30; i++) {
        u = &D_801DEAB4[i];
        if (u->id == -1 || u->state == 1 || u->alpha != 0xFF) {
            continue;
        }
        if (mode == 1 || mode == 4) {
            continue;
        }
        {
            s32 x = u->x >> 2;
            s32 z;
            if (x < (cx - 5) * 32 + 16 || x > (cx + 5) * 32 + 16
                || (z = u->z >> 2, z < (cz - 5) * 32 + 16) || z > (cz + 4) * 32 + 16) {
                dx = (f32)(u->x >> 2) - pos.x;
                dz = (f32)(u->z >> 2) - pos.z;
                /* distance beyond the screen band: x in [-160, 160], z in [-160, 128] */
                if (dx >= 0.0f) {
                    dx = dx - 160.0f;
                } else {
                    dx = -(dx - -160.0f);
                }
                if (dz >= 0.0f) {
                    dz = dz - 128.0f;
                } else {
                    dz = -(dz - -160.0f);
                }
                m = dz;
                if (m < dx) {
                    m = dx;
                }
                if (m >= 48.0f) {
                    m = 256.0f;
                } else {
                    m = m * (256.0 / 48.0);
                }
                if (m < 255.0f) {
                    u->alpha = 255 - (s32)m;
                } else {
                    u->alpha = 0;
                }
            } else {
                u->alpha = 0xFF;
            }
        }
    }
}
