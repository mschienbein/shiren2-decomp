#include "common.h"
typedef struct { short offset; short field_2; s32 (*method)(void *, void *, s32); } Method;
typedef struct { s32 field_0; Method *field_4; } Context;
typedef struct { unsigned char field_0, field_1, field_2; unsigned char field_3[10]; unsigned char field_D; } Item;
typedef struct { Context *field_0; void *field_4; } Pair;
/* Second collection/item link at +0x10 (installed by func_800DB3AC/func_800DB410). */
typedef struct { void *container; Item *item; } ItemLink;
typedef struct { short kind; const void *vtable; Pair field_8; ItemLink field_10; } Object;
extern void *D_801476B8;
extern void func_800AE518(void *, void *, s32, s32);
extern s32 func_800A533C(void *);
s32 func_800DB270(Object *self) {
    Pair *pair = &self->field_8;
    Context *context = pair->field_0;
    s32 result = context->field_4[12].method((unsigned char *)context + context->field_4[12].offset, pair->field_4, 1) ^ 1;
    if (result) return 0;
    {
        Item *item = self->field_10.item;
        if (item->field_0 == 6) item->field_D |= 4;
        func_800AE518(item, D_801476B8, 0, 1);
        if (item->field_0 == 6) item->field_D &= ~4;
        if (!(item->field_2 & 4)) func_800AE518(pair->field_4, D_801476B8, 1, 1);
        func_800A533C(D_801476B8);
    }
    return 0;
}
