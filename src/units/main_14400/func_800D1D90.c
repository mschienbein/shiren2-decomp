#include "common.h"

typedef unsigned char u8;

/* 0x18-byte record (D_80143330[2], cleared by func_800B1080). +4 is the record's
 * area pointer (func_800D2A64 forwards it to func_800A31C8 as a rectangle). */
typedef struct {
    u8 field0;
    u8 pad1[0x3];
    void *field4;
    u8 field8;
    u8 pad9[0x3];
    s32 fieldC;
    s32 field10;
    s32 field14;
} Obj_800D1D90;

/* Whole position pair, copied together by func_800D37E8. */
typedef struct { s32 x, y; } Point;
Point D_80147F70 = { 0, 0 };
static inline void setPointY(Point *point, s32 value) { point->y = value; }

void func_800D1D90(Obj_800D1D90 *obj) {
    obj->field0 = 0;
    obj->field4 = 0;
    obj->field8 = 0;
    obj->fieldC = 0;
    obj->field10 = 0;
    obj->field14 = 0;
    D_80147F70.x = 0;
    setPointY(&D_80147F70, 0);
}
