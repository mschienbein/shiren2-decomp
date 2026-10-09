#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Vec;
typedef struct { Vec min; Vec max; } Rect;
typedef struct { u8 value; } Dir;

extern void *D_80147FE0;
extern Vec D_80148000; /* path start position */
extern Dir D_80148008;
extern Rect D_8014800C;
extern Vec D_80148024;
extern Vec D_80148030;
extern u8 D_80148038;
extern u8 D_80148039;
extern u8 D_8014803C[];
extern u8 D_8014807C[];
extern Rect *D_80148080;
extern u8 D_80148084;
extern u8 D_80148085;

void *func_800B1F90(void *pos);
void *func_800A2594(Vec *out, void *pos, Dir dir);
u32 func_800B1C6C(Vec *pos);
void *func_800A7DEC(void *value);
s32 func_800A4CC4(void *object, void *value, void *direction);
s32 func_800D6318(Vec *out, u8 *tag);
u8 func_800D5DEC(void);

static inline void copyRect(Rect *dst, Rect *src) {
    dst->min = src->min;
    dst->max = src->max;
}

s32 func_800D5BF0(void *owner, Vec *pos, Dir *dir, s32 flag) {
    s32 hit;
    u8 facing;

    D_80147FE0 = owner;
    D_80148000 = *pos;
    D_80148008 = *dir;
    D_80148085 = dir->value;
    D_80148084 = flag;
    D_80148030 = *pos;
    D_8014807C[0] = 0;
    D_80148080 = func_800B1F90(&D_80148000);
    if (D_80148080 == 0) {
        return 1;
    }
    hit = 0;
    {
        Vec probe;
        func_800A2594(&probe, &D_80148000, D_80148008);
        if (func_800B1C6C(&probe) & 0x800) {
            void *who = D_80147FE0;
            hit = func_800A4CC4(who, func_800A7DEC(who), &D_80148008) != 0;
        }
    }
    if (hit) {
        D_80148080 = 0;
    } else {
        copyRect(&D_8014800C, D_80148080);
        D_8014800C.min.y--;
        D_8014800C.min.x--;
        D_8014800C.max.y++;
        D_8014800C.max.x++;
    }
    if (D_80148080 == 0) {
        return 1;
    }
    D_80148039 = 0;
    hit = 0;
    if (func_800D6318(&D_80148024, D_8014807C)) {
        hit = 0 < func_800D5DEC();
    }
    if (hit) {
        return 1;
    }
    facing = dir->value;
    D_80148038 = 1;
    D_8014803C[0] = facing;
    return 0;
}
