#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* g++ 2.x vtable slot: this-adjustment delta, index, function pointer. */
typedef struct {
    s16 delta;
    s16 index;
    void *(*fn)(void *self);
} VtblEntry;

typedef struct {
    u8 pad0[0x20];
    u8 count_20;
} Obj_8010EDD4;

typedef struct {
    u8 pad0[0x1E];
    u8 flags_1E;
    u8 pad1F[0x5];
    VtblEntry *vtbl_24;
} Other_8010EDD4;

extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 message_id, ...);
extern char *func_800AE674(void *obj);
extern void func_800D3650(Obj_8010EDD4 *obj);
extern void func_800CD364(void *target, Obj_8010EDD4 *obj);

s32 func_8010EDD4(Obj_8010EDD4 *obj, Other_8010EDD4 *other) {
    VtblEntry *entry;
    void *target;

    if (--obj->count_20 == 0) {
        func_800498E4(0xFF, func_800AE674(obj));
        func_80049CB4(0x128, 0x17);
        if ((other->flags_1E >> 2) & 1) {
            func_800D3650(obj);
        }
        entry = &other->vtbl_24[19];
        target = entry->fn((u8 *)other + entry->delta);
        if (target != 0) {
            func_800CD364(target, obj);
        }
        return 1;
    }
    return 0;
}
