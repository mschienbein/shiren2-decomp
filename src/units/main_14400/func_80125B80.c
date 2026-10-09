#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 x; s32 y; } Pos80125B80;
typedef struct { u8 value; } Dir80125B80;

/* Throw request passed to the projectile's method slot 0x3C. */
typedef struct {
    s32 kind;
    void *source;
    s32 pad08;
    Dir80125B80 dir;
    Pos80125B80 pos;
    s32 range;
    s32 flags;
} Throw80125B80;

typedef struct {
    u8 pad0[0x38];
    s16 delta_38;
    s16 pad3A;
    s32 (*launch_3C)(void *self, Throw80125B80 *request);
} VTable80125B80;

typedef struct Obj800BCB18 {
    u8 pad0[8];
    VTable80125B80 *vtable_08;
    u8 padC[4];
    s32 kind_10;
    u8 power_14;
    u8 count_15;
} Obj800BCB18;

extern u16 D_80156A70;

void *func_8011D378(void);
s32 func_800AC670(Obj800BCB18 *obj);
u16 func_80115944(void *owner, u16 value);
void *func_801156DC(void *out, void *owner, void *position, void *dir, s32 range);
void *func_800B31E8(void *pos, s32 team);
s32 func_80049CB4(s32 id, ...);
void func_80115E18(void *obj, void *position);
void func_800498E4(s32 id, ...);

static inline void reverse_dir(Dir80125B80 *out, Dir80125B80 *dir)
{
    out->value = (dir->value + 4) & 7;
}

/* Slot 0x44 receives seven pointers; target and item are supplied but unused. */
s32 func_80125B80(void *owner, void *a, void *b, void *c, Dir80125B80 *facing, void *target, void *item)
{
    Pos80125B80 pos;
    Throw80125B80 request;
    Dir80125B80 dir;
    Dir80125B80 back;
    Throw80125B80 *req;
    Obj800BCB18 *obj = func_8011D378();
    s32 usable = func_800AC670(obj) ^ 1;

    if (usable) {
        s32 flags;

        obj->power_14 = func_80115944(owner, D_80156A70);
        flags = 4;
        obj->count_15 = 1;
        obj->kind_10 = 0x11;
        dir.value = (facing->value - 2) & 7;
        func_801156DC(&pos, owner, c, &dir, 100);
        if (func_800B31E8(c, 2) != 0) {
            flags = 0x24;
        }
        func_80049CB4(0x125, c);
        func_80115E18(owner, b);
        request.kind = 0x11;
        request.source = a;
        reverse_dir(&back, &dir);
        request.pos = pos;
        request.dir = back;
        req = &request;
        req->range = 0xFF;
        request.flags = flags;
        obj->vtable_08->launch_3C((u8 *)obj + obj->vtable_08->delta_38, req);
    } else {
        func_800498E4(0x223);
    }
    return 1;
}
