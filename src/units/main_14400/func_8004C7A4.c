#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair position; } Unit;
typedef struct {
    u8 pad_00[0x12];
    u16 field_12;
    u8 pad_14[0x10];
    s32 field_24;
    u8 pad_28[0x34];
    s32 field_5C;
    u8 pad_60[8];
    s32 field_68;
} Effect;
extern s32 D_8013968C;
extern u8 func_800A8C00(void *actor);
extern void func_80084A20(void);
extern void func_80050E44(s32 id, Pair *cell);
extern void func_80084B80(void);
extern void func_8008AF00(void *obj);
extern void func_80088E70(void *obj);
extern void func_80089634(void *obj);
extern void *func_80085154(void (*handler)(void *), s32 value);
extern Unit *func_800C5F60(void);
extern void func_80051264(s32 id, Unit *obj, s32 flags);
static inline Pair *copy_position(Pair *out, const Pair *position) {
    out->x = position->x;
    out->y = position->y;
    return out;
}


void func_8004C7A4(Unit *first, Unit *second) {
    Pair position1, position2;
    Pair *copied;
    Effect *effect;
    s32 track1 = func_800A8C00(first);
    s32 track2 = func_800A8C00(second);
    switch (D_8013968C) {
    case 0x1A:
        copied = copy_position(&position1, &first->position);
        position2.x = second->position.x;
        position2.y = second->position.y;
        func_80084A20();
        func_80050E44(0x1AF, copied);
        func_80084B80();
        func_80084A20();
        func_80085154(func_8008AF00, 0x1F);
        func_80050E44(0x1AF, &position2);
        func_80084B80();
        effect = func_80085154(func_80088E70, track1);
        effect->field_24 = track2;
        if (first == func_800C5F60()) {
            effect->field_12 |= 0x4000;
            effect->field_5C = position1.y;
            effect->field_68 = position1.x;
        } else if (second == func_800C5F60()) {
            effect->field_12 |= 0x4000;
            effect->field_5C = position2.y;
            effect->field_68 = position2.x;
        }
        break;
    case 0x5D:
        func_80051264(0xEA, first, 0x80);
        effect = func_80085154(func_80089634, track2);
        effect->field_24 = track1;
        break;
    }
}
