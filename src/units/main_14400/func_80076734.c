#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef float f32;
typedef struct { s16 x; s16 z; s16 pad; } Pt80076734;
typedef struct { s8 count; u8 pad1; Pt80076734 a[3]; Pt80076734 b[3]; u8 pad26[0x8]; } Zone80076734;
typedef struct { u8 pad0[0x2]; s16 id; u8 pad4[0x4]; u8 state; u8 pad9[0x3]; s16 x; u8 padE[0x2]; s16 z; u8 pad12[0x9E]; } Actor80076734;
typedef struct { f32 x; f32 y; f32 z; } Vec80076734;
extern s32 D_8013D8CC;
s32 D_8013D8E4 = 1;
extern s32 D_8013D8E8;
extern s32 D_8013D8F0;
extern s32 D_8013D8FC;
extern s32 D_801E4E70;
extern Zone80076734 D_801A79E8[];
extern Actor80076734 D_801DEAB4[];
void func_80061AB8(s32 arg0, s32 arg1, f32 *minX, f32 *minZ, f32 *maxX, f32 *maxZ);
s32 func_80074114(void);
void func_800593F8(Vec80076734 *pos);
s32 func_80041FF8(void);
s32 func_8005B2CC(void);
void func_8005ADAC(s32 arg0, s32 arg1, s32 arg2);
s32 func_800627C4(void);
void func_80077498(s32 arg0);
void func_80076F48(s32 arg0);
s32 func_80084E20(void);
void func_8006E6D8(void);
void func_800765AC(void);
static inline s32 clampMin(s32 value, s32 limit) {
    return (value < limit) ? limit : value;
}

static inline s32 clampMax(s32 value, s32 limit) {
    return (value > limit) ? limit : value;
}

void func_80076734(s32 force, s32 arg1) {
    Vec80076734 pos;
    f32 minX;
    f32 minZ;
    f32 maxX;
    f32 maxZ;
    s32 ok = 0;
    s32 n = 1;
    s32 idx;
    u32 off; /* byte offset of this zone; kept separate to match codegen */
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 cx;
    s32 cz;
    s32 i;
    s32 j;
    s32 k;
    s32 count;
    Zone80076734 *zone;
    Actor80076734 *actor;

    if (D_8013D8CC == 0) {
        return;
    }
    func_80061AB8(0, 0, &minX, &minZ, &maxX, &maxZ);
    D_8013D8E4 = 0;
    idx = func_80074114();
    off = idx * sizeof(Zone80076734);
    func_800593F8(&pos);
    if (((Zone80076734 *)((u8 *)D_801A79E8 + off))->count != 0) {
        if ((u8)func_80041FF8() == 0 && func_8005B2CC() == 0) {
            func_8005ADAC(8, 1, 1);
        }
    } else {
        ok = 1;
        switch ((u32)func_800627C4()) {
        case 1:
            left = -10;
            right = 10;
            top = -10;
            bottom = 8;
            cx = (s32)pos.x >> 5;
            cz = (s32)pos.z >> 5;
            break;
        case 3:
            cx = D_801DEAB4[idx].x >> 7;
            cz = D_801DEAB4[idx].z >> 7;
            if (D_801E4E70 == 0 || D_8013D8F0 == 1 || D_8013D8FC == 1) {
                left = -5;
                right = 5;
                top = -5;
                bottom = 4;
            } else {
                left = (s32)((minX - 32.0f) * 0.03125f) - cx;
                right = (s32)((maxX + 32.0f) * 0.03125f) - cx;
                top = (s32)((minZ - 32.0f) * 0.03125f) - cz;
                bottom = (s32)((maxZ + 32.0f) * 0.03125f) - cz;
                left = clampMin(left, -5);
                top = clampMin(top, -5);
                right = clampMax(right, 5);
                bottom = clampMax(bottom, 4);
            }
            break;
        case 2:
        default:
            left = -5;
            right = 5;
            top = -5;
            bottom = 4;
            cx = D_801DEAB4[idx].x >> 7;
            cz = D_801DEAB4[idx].z >> 7;
            break;
        }
        for (i = 0; i < 30; i++) {
            zone = &D_801A79E8[i];
            actor = &D_801DEAB4[i];
            if (actor->id == -1) {
                continue;
            }
            if (zone->count == 0) {
                continue;
            }
            if (D_801DEAB4[i].state == 1) {
                continue;
            }
            count = (zone->count < 0) ? 1 : zone->count;
            for (j = 0; j < count; j++) {
                if ((zone->a[j].x >= cx + left && zone->a[j].x <= cx + right
                     && zone->a[j].z >= cz + top && zone->a[j].z <= cz + bottom)
                    || (zone->b[j].x >= cx + left && zone->b[j].x <= cx + right
                        && zone->b[j].z >= cz + top && zone->b[j].z <= cz + bottom)) {
                    ok = 0;
                    break;
                }
            }
            if (ok == 0) {
                break;
            }
        }
    }
    if ((u8)func_80041FF8() != 0) {
        ok = 0;
    }
    func_80077498(0);
    if (force != 0 || ok != 0) {
        D_8013D8E4 = force ? 1 : 2;
        func_80076F48(8);
    } else {
        n = 1;
        if (D_8013D8E8 == 1) {
            n = 2;
        }
        j = 1;
        while (1) {
            D_8013D8E4 = 1;
            for (k = 0; k < n; k++) {
                func_80076F48(j);
                j++;
            }
            if (arg1 != 0) {
                func_80084E20();
            }
            if (j >= 9) {
                break;
            }
            func_8006E6D8();
        }
    }
    func_80077498(1);
    func_8006E6D8();
    func_800765AC();
}
