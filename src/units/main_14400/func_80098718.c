#include "common.h"
typedef unsigned char u8;
/* Item record: kind at +1 (0xF1 = stacked item), width at +4, stacked width at +0x10. */
typedef struct { u8 type, kind; u8 pad2[2]; u8 field4; u8 pad5[11]; u8 field10; } Item;
/* Item-set vtable (vptr at +4): +0x24 count, +0x3C getter (decided item-set contracts). */
typedef struct {
    u8 pad0[0x20];
    short delta20, pad22;
    s32 (*count24)(void *self);
    u8 pad28[0x10];
    short delta38, pad3A;
    void *(*at3C)(void *self, u32 index);
} VTable;
typedef struct { void *owner; VTable *vtable; } Container;
/* Grid menu: three item groups, per-cell widths, then running count/width/line totals. */
typedef struct {
    u8 pad0[0x24];
    s32 field24;
    u8 pad28[0x2C8];
    Container *containers[3];
    s32 mode;
    u8 widths[3][60];
    s32 counts[4], totals[4], lines[4];
} Object;
extern s32 func_80098DD4(Object *object, s32 id);

static __inline__ s32 item_width(Item *item) { return item->field4; }
static __inline__ s32 container_count(Container *container) {
    return container->vtable->count24((char *)container + container->vtable->delta20);
}

void func_80098718(Object *object) {
    s32 group;
    object->counts[0] = object->totals[0] = object->lines[0] = 0;
    group = 0;
    for (;;) {
        s32 next;
        Container *container;
        if (group >= 3) break;
        next = group + 1;
        object->counts[next] = object->counts[group];
        object->totals[next] = object->totals[group];
        object->lines[next] = object->lines[group];
        container = object->containers[group];
        if (container != 0) {
            s32 count = container_count(container);
            s32 index, width;
            object->counts[next] += count;
            index = 0;
            for (;;) {
                Item *item;
                Item *plain;
                Container *list;
                if (index >= count) break;
                list = object->containers[group];
                item = list->vtable->at3C((char *)list + list->vtable->delta38, index);
                /* ODD_C: the plain-width read goes through a second pointer to the
                   item; the ROM keeps that copy in its own register (a1). */
                plain = item;
                if (!item) object->widths[group][index] = 0;
                else object->widths[group][index] = item->kind == 0xF1 ? item->field10 : item_width(plain);
                object->totals[next] += object->widths[group][index];
                index++;
            }
            width = object->totals[next] - object->totals[group];
            if (width > 0) object->lines[next] += 1 + (width - func_80098DD4(object, object->counts[next] - 1)) / 10;
            else object->lines[next]++;
        } else if (group == 0) object->lines[1] = 1;
        group++;
    }
    object->field24 = object->lines[3];
}
