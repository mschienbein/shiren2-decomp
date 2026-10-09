#include "common.h"

typedef struct { s32 field00, field04; } Position;
typedef struct { Position first, last; } Bounds;
typedef struct { unsigned char unknown00[0x3dc]; s32 field3dc; } Object;
extern unsigned char D_80156AD5;
extern signed char D_80143392, D_8014344C;
extern char D_80147620[];
extern unsigned char func_800C57CC(void *, s32), func_800C57A0(void *);
extern void func_800B7948(Object *, s32, Bounds *);
extern void func_800B17A4(void);
void func_800C10F0(Object *object) {
    Bounds bounds, copy, final;
    s32 offset;
    s32 range = D_80156AD5;
    bounds.first.field00 = 0x12;
    bounds.first.field04 = 0x18;
    bounds.last.field00 = 0x22;
    bounds.last.field04 = 0x33;
    offset = func_800C57CC(D_80147620, range) & 0xff;
    if (func_800C57A0(D_80147620) & 1) offset = -offset;
    bounds.first.field04 += offset;
    offset = func_800C57CC(D_80147620, range) & 0xff;
    if (func_800C57A0(D_80147620) & 1) offset = -offset;
    bounds.first.field00 += offset;
    offset = func_800C57CC(D_80147620, range) & 0xff;
    if (func_800C57A0(D_80147620) & 1) offset = -offset;
    bounds.last.field04 += offset;
    offset = func_800C57CC(D_80147620, range) & 0xff;
    if (func_800C57A0(D_80147620) & 1) offset = -offset;
    bounds.last.field00 += offset;
    copy.first = bounds.first;
    copy.last = bounds.last;
    final.first.field00 = copy.first.field00;
    final.first.field04 = copy.first.field04;
    final.last.field00 = copy.last.field00;
    final.last.field04 = copy.last.field04;
    func_800B7948(object, 0, &final);
    object->field3dc = 1;
    D_8014344C = 1;
    func_800B17A4();
    D_80143392 = 5;
}
