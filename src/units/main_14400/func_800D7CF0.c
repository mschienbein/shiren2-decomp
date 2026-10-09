#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0x9A];
    u16 flags9A;
} Obj800D7CF0;

extern u8 D_80148190[];
extern s32 func_800D8088(u8 a, u8 b);
extern void *func_800A8694(u8 a, u8 b, void *memory);
extern s32 func_800D7D84(u8 kind, u8 level);
extern void func_800F04EC(Obj800D7CF0 *obj, s16 delta);

Obj800D7CF0 *func_800D7CF0(u8 kind, u8 level) {
    Obj800D7CF0 *obj = 0;

    if (func_800D8088(kind, level) != 0) {
        obj = func_800A8694(kind, level, 0);
        if (obj != 0) {
            obj->flags9A |= 0x40;
            func_800F04EC(obj, D_80148190[func_800D7D84(kind, level)] - 1);
        }
    }
    return obj;
}
