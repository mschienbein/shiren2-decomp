#include "common.h"

typedef struct Pos Pos;
typedef Pos Cell;
extern u32 D_8013968C;
extern s32 D_80139690;
extern void func_8008AF94(void *task);
extern void *func_80085154(void (*handler)(void *), s32 value);
extern void func_80050E44(s32 effect, Cell *position);
extern void func_80050B3C(s32 effect, Pos *position, s32 value);
extern void func_80050F34(s32 effect, Pos *position, s32 direction, s32 flags);

void func_800504DC(Pos *position, s32 variant)
{
    switch (D_8013968C) {
    case 0xF7:
        switch (variant) {
        case 1:
            func_80085154(func_8008AF94, 0x51);
            func_80050E44(0x97, position);
            break;
        case 2:
            func_80085154(func_8008AF94, 0x52);
            func_80050E44(0x98, position);
            break;
        case 3:
            func_80085154(func_8008AF94, 0x53);
            func_80050E44(0x99, position);
            break;
        }
        break;
    case 0x11E:
        if (!variant)
            func_80050E44(0xA7, position);
        else
            func_80050E44(0xAA, position);
        break;
    case 0x11F:
        if (D_80139690)
            func_80050F34(0x37, position, (variant + 4) % 8, 0);
        else
            func_80050B3C(0x37, position, 5);
        break;
    case 0x11A:
        if (!(variant & 1))
            func_80050F34(0xA5, position, (variant + 4) % 8, 0);
        break;
    }
}
