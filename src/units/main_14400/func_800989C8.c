#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* g++ 2.x vtable slot 14: name getter (e.g. func_800D055C). */
typedef struct {
    s16 delta;
    s16 index;
    char *(*fn)(void *self);
} VtblEntry;

typedef struct {
    u8 pad0[0x4];
    VtblEntry *vtbl4;
} Named;

typedef struct {
    u8 data[0x20];
} Text;

typedef struct {
    u8 pad0[0x38];
    s32 value38;
    u8 pad3C[0x2B4];
    Named *items2F0[3];
    s32 kind2FC;
    u8 pad300[0xD4];
    /* Cumulative boundaries, including the zero boundary at 0x3D4. */
    s32 thresholds3D4[4];
    u8 pad3E4[0x4];
    Text text3E8;
    u16 message408;
} Obj_800989C8;

void func_80048764(Text *text);
char *func_80048480(u16 id);
void func_800487EC(Text *text, s32 style, s32 flags, char *str);

static inline char *item_name(Named *item) {
    VtblEntry *entry = &item->vtbl4[14];

    return entry->fn((u8 *)item + entry->delta);
}

/* Print the description line for the current selection kind. */
void func_800989C8(Obj_800989C8 *obj) {
    s32 i;

    switch (obj->kind2FC) {
    case 0x20:
        func_80048764(&obj->text3E8);
        if (obj->items2F0[1] != 0 && obj->value38 >= obj->thresholds3D4[1]) {
            func_800487EC(&obj->text3E8, 0, 0, func_80048480(0x466));
        } else {
            func_800487EC(&obj->text3E8, 0, 0, func_80048480(0x24D));
        }
        break;
    case 0x4:
    case 0x8:
    case 0x10:
    case 0x2000:
    case 0x4000:
    case 0xC000:
        func_80048764(&obj->text3E8);
        func_800487EC(&obj->text3E8, 0, 1, item_name(obj->items2F0[0]));
        break;
    case 0x400:
        func_80048764(&obj->text3E8);
        for (i = 0; ; i++) {
            s32 more = i < 3;
            if (!more) {
                break;
            }
            if (obj->value38 < obj->thresholds3D4[i + 1]) {
                func_800487EC(&obj->text3E8, 0, 0, item_name(obj->items2F0[i]));
                return;
            }
        }
        func_800487EC(&obj->text3E8, 0, 0, func_80048480(0x467));
        break;
    default:
        if (obj->message408 != 0) {
            func_80048764(&obj->text3E8);
            func_800487EC(&obj->text3E8, 0, 0, func_80048480(obj->message408));
        }
        break;
    }
}
