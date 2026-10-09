#include "common.h"
typedef unsigned char u8;
typedef struct Unit { s32 x; s32 y; u8 pad_08[0xFC]; struct Unit *linked_104; } Unit;
typedef struct Effect { u8 pad_00[0x24]; s32 y_24; s32 x_28; u8 pad_2C[0x38]; s32 y_64; u8 pad_68[8]; s32 x_70; } Effect;
extern u32 D_8013968C;
extern Unit *D_801476B8;
extern void *func_80085154(void (*handler)(void *), s32 value);
extern void func_800869E8(void *);
extern void func_80086A68(void *);
extern void func_80086A28(void *);
extern void func_8008B4B0(void *);
extern u8 func_800A8C00(void *actor);
extern Unit *func_800C5F60(void);
void func_800506E8(void) {
    Effect *effect;
    Unit *unit;
    switch (D_8013968C) {
    case 0xD9:
        func_80085154(func_800869E8, 1);
        break;
    case 0xDD:
        unit = D_801476B8->linked_104;
        if (!unit) unit = D_801476B8;
        func_80085154(func_80086A68, func_800A8C00(unit));
        break;
    case 0xDE:
        func_80085154(func_80086A28, 0);
        break;
    case 0xDB:
        effect = func_80085154(func_8008B4B0, 0);
        effect->y_64 = D_801476B8->y;
        effect->x_70 = D_801476B8->x;
        effect->y_24 = func_800C5F60()->y;
        effect->x_28 = func_800C5F60()->x;
        break;
    }
}
