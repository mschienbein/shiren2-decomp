#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad[0x14]; s32 x14; u8 x18[0x10]; s32 x28; s32 pad2C; s32 x30; s32 pad34; s32 x38; s32 pad3C; } Shape;
typedef struct { u8 pad[2]; u8 count; u8 pad3[5]; Shape **items; } ShapeList;
void *func_80091450(u32);
u8 *func_8006A810(u8 *, s32, s32);
void func_8008D3A0(void *, s32, void (*)(void), s32 (*)(void *, void *, void *), void (*)(void *));
s32 func_8008DF04(void *);
s32 func_8008F380(void *, void *);
void func_80091544(void *);
void func_8008F1E0(void);
s32 func_8008F1E8(void *, void *, void *);
void func_8008F378(void *);
s32 func_8008F410(void *ctx, s32 size, ShapeList *list) {
    s32 ret = 0;
    s32 n;
    Shape *shape;
    /* ODD_C: single-pass error block grouping shape allocation and nested-parser size
     * validation before the shared cleanup; it also shapes the prologue scheduling. */
    do {
        shape = func_80091450(0x40);
        if (shape == 0) {
            ret = -1;
            break;
        }
        func_8006A810((u8 *)shape, 0, 0x40);
        func_8008D3A0(shape, 0x53484150, func_8008F1E0, func_8008F1E8, func_8008F378);
        shape->x14 = func_8008DF04(ctx);
        n = func_8008F380(ctx, shape->x18);
        shape->x28 = func_8008DF04(ctx);
        shape->x30 = func_8008DF04(ctx);
        shape->x38 = func_8008DF04(ctx);
        if (n + 0x10 != size) {
            ret = -1;
            break;
        }
        list->items[list->count++] = shape;
    } while (0);
    if (ret != 0 && shape != 0) func_80091544(shape);
    return ret;
}
