#include "common.h"
#include "map_rect.h"

typedef struct { unsigned char unknown00[0x3dc]; s32 field3dc; } Object;
extern unsigned char D_80156AD5;
extern signed char D_80143392, D_8014344C;
extern char D_80147620[];
extern unsigned char func_800C57CC(void *, s32), func_800C57A0(void *);
extern void func_800B17A4(void);
void func_800C10F0(Object *object) {
    MapRect bounds, copy, final;
    s32 offset;
    s32 range = D_80156AD5;
    bounds.min.x = 0x12;
    bounds.min.y = 0x18;
    bounds.max.x = 0x22;
    bounds.max.y = 0x33;
    offset = func_800C57CC(D_80147620, range) & 0xff;
    if (func_800C57A0(D_80147620) & 1) offset = -offset;
    bounds.min.y += offset;
    offset = func_800C57CC(D_80147620, range) & 0xff;
    if (func_800C57A0(D_80147620) & 1) offset = -offset;
    bounds.min.x += offset;
    offset = func_800C57CC(D_80147620, range) & 0xff;
    if (func_800C57A0(D_80147620) & 1) offset = -offset;
    bounds.max.y += offset;
    offset = func_800C57CC(D_80147620, range) & 0xff;
    if (func_800C57A0(D_80147620) & 1) offset = -offset;
    bounds.max.x += offset;
    copy.min = bounds.min;
    copy.max = bounds.max;
    final.min.x = copy.min.x;
    final.min.y = copy.min.y;
    final.max.x = copy.max.x;
    final.max.y = copy.max.y;
    func_800B7948(object, 0, &final);
    object->field3dc = 1;
    D_8014344C = 1;
    func_800B17A4();
    D_80143392 = 5;
}
