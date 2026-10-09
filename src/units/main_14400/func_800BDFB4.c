#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair first, second; } Rect;
typedef struct { u8 field_00[0x402]; u16 field_402; u8 field_404[0x554]; u16 field_958; } Object;
extern u8 D_80147620[];
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern Pair *func_800A2544(Pair *out, Pair *a, Pair *b);
extern Pair *func_800A256C(Pair *out, Pair *a, Pair *b);
extern Pair *func_800A33DC(Pair *out, Rect *rect);
extern void func_800B834C(void *ctx, s32 r, u16 color);
static inline Pair *set_pair(Pair *pair, s32 x, s32 y) {
    pair->x = x;
    pair->y = y;
    return pair;
}
s32 func_800BDFB4(Object *object) {
    Rect rect;
    Pair result, low, low_radius, high_result, high, high_radius;
    s32 radius;
    if (!(object->field_958 & 0x100)) return 0;
    {
        radius = (u8)func_800C5844(D_80147620, 7, 16);
        set_pair(&low, 10, 10);
        low_radius.x = radius;
        low_radius.y = radius;
        func_800A2544(&result, &low, &low_radius);
        set_pair(&high, 43, 65);
        high_radius.x = radius;
        high_radius.y = radius;
        func_800A256C(&high_result, &high, &high_radius);
        rect.first = result;
        rect.second = high_result;
        func_800A33DC(&result, &rect);
        func_800B834C(&result, radius, object->field_402);
        return 1;
    }
}
