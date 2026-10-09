#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

typedef struct ShirenDirection { s8 value; } ShirenDirection;

/* Partial view of the acting unit: facing direction at 0x08. */
typedef struct Unit80119730 {
    u8 pad_00[8];
    ShirenDirection dir_08;
} Unit80119730;

/* 0x50-byte effect record filled by func_800C4360/func_800C2D0C; the hit target is at 0x40. */
typedef struct Effect80119730 {
    u8 pad_00[0x40];
    void *target_40;
    u8 pad_44[0xC];
} Effect80119730;

extern u32 D_8013960C;
extern u16 D_801569EA;

s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 id, ...);
void *func_800C4360(void *self, void *owner, u16 value, s32 command, void *position,
                    ShirenDirection direction, s32 limit, u16 flags, u8 mode);
void func_800C2D0C(Effect80119730 *effect);
char *func_800A3B20(void *u);
s32 func_800E1CC4(void *obj, s32 kind);
void func_80119670(void *self, void *actor, void *target, short amount);

/* Virtual method (vtable D_8015E360 slot 0x44): the dispatchers supply (self, unit). */
void func_80119730(void *self, Unit80119730 *actor)
{
    s32 scale;
    void *hit;
    Effect80119730 effect;
    ShirenDirection dir;
    Unit80119730 *victim;

    func_80049CB4(0x46);
    D_8013960C <<= 1;
    dir = actor->dir_08;
    func_800C4360(&effect, actor, 0, 0x100, actor, dir, 0xFF, 0x811, 0);
    func_800C2D0C(&effect);
    D_8013960C >>= 1;
    hit = effect.target_40;
    victim = hit;
    if (victim != 0) {
        func_800498E4(0x11E, func_800A3B20(victim));
        if (func_800E1CC4(actor, 3)) {
            scale = 2;
        } else {
            scale = 1;
        }
        func_80119670(self, actor, victim, D_801569EA * scale);
    }
}
