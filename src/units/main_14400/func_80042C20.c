#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { short offset; short field_2; void (*method)(void *); } Method;
typedef struct { s32 field_0, field_4; Method *field_8; s32 field_C; } Object;
typedef struct { unsigned short field_0, field_2; s32 field_4; Point *field_8; s32 field_C[2]; } Row;
typedef struct { s32 count; Row *rows; } Rows;
typedef struct { s32 field_0; Rows *field_4; } Context;
typedef struct { unsigned char field_0[0xC]; unsigned short field_C; } Item;
extern unsigned short D_80143450[];
extern Context *func_80066B20(void);
extern void func_800B1080(void);
extern void *func_800AC5B4(s32, s32);
extern Item *func_8010DD70(void *, s32);
extern s32 func_800AC670(Item *);
extern s32 func_800AD8AC(Item *, Point *);
extern s32 func_80049CB4(s32, ...);
static inline Point *init_point(Point *dest, s32 x, s32 y) { dest->x = x; dest->y = y; return dest; }
void func_80042C20(Object *self) {
    Context *context = func_80066B20();
    unsigned short *entry;
    s32 i, j;
    func_800B1080();
    entry = D_80143450;
    { s32 count; for (count = 0x1007; count >= 0; --count) *entry++ = 0; }
    self->field_8[2].method((unsigned char *)self + self->field_8[2].offset);
    if (!self->field_4) {
        for (i = 0; ; ++i) {
            Rows *rows = context->field_4;
            Row *row;
            if (i >= rows->count) break;
            if (self->field_C >= 0 && i != self->field_C) continue;
            row = &rows->rows[i];
            for (j = 0; ; ++j) {
                Item *item;
                Point point;
                if (j >= row->field_4) break;
                item = func_8010DD70(func_800AC5B4(0x10, 1), 0xCE);
                item->field_C = row->field_2;
                if (func_800AC670(item)) continue;
                func_800AD8AC(item, init_point(&point, row->field_8[j].y, row->field_8[j].x));
            }
        }
    }
    func_80049CB4(5, 1);
}
