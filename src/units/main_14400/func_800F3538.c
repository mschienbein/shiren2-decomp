#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { s32 x, y; } Point;
typedef struct { u8 value; } Dir;
typedef struct { Point position; u8 pad8[0x6A]; u8 flags_72; } Unit;
typedef Unit Entity;
typedef Unit Obj800F1568;
typedef struct { s32 type; Unit *source; void *field_8; Dir direction; u8 padD[3]; Point position; s32 field_18; void *field_1C; } Message;
typedef struct { u8 pad0[8]; s16 destroy_delta; s16 destroy_index; void (*destroy)(void *, s32); u8 pad10[0x28]; s16 dispatch_delta; s16 dispatch_index; s32 (*dispatch)(void *, Message *); } Vtable;
typedef struct { u8 type; u8 pad1[7]; Vtable *vtable; } Item800F1568;
extern s32 func_800A692C(Entity *, s32);
extern Item800F1568 *func_800F1568(Obj800F1568 *, s32);
extern void *func_800AAF38(void);
extern void *func_800AAF68(void);
extern void *func_800AAF98(void);
extern s32 func_80049CB4(s32, ...);
extern char *func_800A3B20(Unit *);
extern char *func_800AE674(void *);
extern void func_800498E4(s32, ...);
static inline void copy_position(Point *out, Unit *obj) { out->x = obj->position.x; out->y = obj->position.y; }
static inline void copy_direction(Dir *out, Dir *in) { out->value = in->value; }
s32 func_800F3538(Unit *obj, Unit *source, s32 kind, Dir direction) {
    Point position;
    Message message;
    s32 denied = 0;
    Item800F1568 *item;
    if (!(obj->flags_72 & 8) || func_800A692C(obj, 18)) denied = 1;
    if (denied) return 0;
    item = func_800F1568(obj, 0);
    if (item) {
        if (item->type != kind) {
            item->vtable->destroy((u8 *)item + item->vtable->destroy_delta, 3);
            item = 0;
        }
    }
    if (!item) {
        switch (kind) {
        case 3: case 9: item = func_800AAF38(); break;
        case 4: item = func_800AAF68(); break;
        case 6: item = func_800AAF98(); break;
        default: return 0;
        }
    }
    if (item) {
        Point *p = &position;
        Message *packet;
        copy_position(p, obj);
        if (func_80049CB4(0xDA, p) != -2) {
            char *name;
            char *itemName;
            func_80049CB4(0x124, p);
            name = func_800A3B20(source);
            itemName = func_800AE674(item);
            func_800498E4(0x55, name, itemName);
        }
        message.type = 0x11;
        message.source = source;
        message.position = position;
        copy_direction(&message.direction, &direction);
        packet = &message;
        packet->field_18 = -1;
        message.field_1C = 0;
        item->vtable->dispatch((u8 *)item + item->vtable->dispatch_delta, packet);
        obj->flags_72 &= ~8;
        return 1;
    }
    return 0;
}
