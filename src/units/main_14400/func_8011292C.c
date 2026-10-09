#include "common.h"

typedef unsigned short u16;
typedef struct {
    unsigned char pad_00;
    unsigned char id_01;
    unsigned char pad_02[0xA];
    s32 mode_0C;
} Item;
extern const u16 D_801578A0[];
extern const u16 D_80156A46;
extern const u16 D_80156A4A;
extern s32 func_800AC584(u16 value);

/* Item price: base price scaled by a per-mode rate expressed in tenths. */
s32 func_8011292C(Item *item)
{
    s32 price = func_800AC584(D_801578A0[item->id_01 - 0xE9]);
    u16 rate;

    switch (item->mode_0C) {
    case 2: rate = D_80156A46; break;
    case 3: rate = D_80156A4A; break;
    default: rate = 10; break;
    }
    price = price * rate / 10;
    return price;
}
