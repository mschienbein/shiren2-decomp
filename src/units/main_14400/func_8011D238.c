#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct { u8 pad0[0x18]; s16 x18; s16 pad1A; void (*x1C)(void *); } VTable;
typedef struct {
    u8 pad0[0x1E];
    u8 x1E;
    u8 pad1F[5];
    VTable *vtable;
} Unit;
extern u16 D_8014767C;
extern s32 D_80147678;
s32 func_800A99D0(void);
s32 func_80049CB4(s32 id, ...);
void func_80049A04(u16 message_id, ...);
char *func_800A3B20(Unit *unit);
void func_800498E4(s32 message_id, ...);
void func_80049BF0(s32 arg);
/* Item-effect slot +0x44 supplies self, actor and item; self and item are unused here. */
void func_8011D238(void *obj, Unit *unit, void *item) {
    s32 busy = 0;
    if ((D_8014767C & 0xC) || func_800A99D0()) {
        busy = 1;
    }
    if (busy) {
        func_80049CB4(0x132);
        func_80049A04(0x225);
    } else {
        func_80049CB4(0x132);
        func_80049CB4(0x77, unit);
        if ((unit->x1E >> 2) & 1) {
            D_80147678 = 3;
            func_800498E4(0xE1, func_800A3B20(unit));
            func_80049BF0(0);
        } else {
            unit->vtable->x1C((u8 *)unit + unit->vtable->x18);
        }
    }
}
