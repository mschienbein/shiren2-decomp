#include "common.h"

typedef struct Cell Cell;
typedef struct { short id; unsigned char mode; unsigned char pad3; } Entry;
typedef struct {
    unsigned char pad0[0x1C];
    s32 field1C;
    unsigned char pad20[4];
    s32 field24;
} Object;
extern s32 D_80139690;
extern Entry D_8013A9E0[];
extern void func_80050E44(s32 arg0, Cell *cell);
extern void *func_800851B0(s32 id);

void func_80050B3C(s32 index, Cell *cell, s32 value)
{
    if (D_80139690 != 0) {
        func_80050E44(index, cell);
    } else {
        Object *object = func_800851B0(D_8013A9E0[index].id);
        s32 mode = D_8013A9E0[index].mode;
        object->field24 = value;
        object->field1C = mode;
    }
}
