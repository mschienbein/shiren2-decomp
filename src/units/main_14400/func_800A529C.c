#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 x, y; } Pos;

typedef struct {
    u8 pad0[0x18];
    s16 delta_18;
    s16 index_1A;
    void (*method_1C)(void *self);
} VTable800A529C;

typedef struct {
    Pos pos;
    u8 pad8[0x1C - 8];
    u16 flags_1C;
    u8 pad1E[0x24 - 0x1E];
    VTable800A529C *vtable;
} Obj800A529C;

typedef struct {
    Pos pos;
} Unit800A529C;

extern Unit800A529C *func_800C5F60(void);
extern s32 func_800A60D8(Obj800A529C *obj, Pos *pos);
extern s32 func_800A5168(Obj800A529C *obj, Pos *pos, s32 flag);

static inline void position(Pos *out, Unit800A529C *unit) {
    out->x = unit->pos.x;
    out->y = unit->pos.y;
}

s32 func_800A529C(Obj800A529C *obj, s32 flag) {
    Pos pos;
    s32 ok;

    position(&pos, func_800C5F60());
    ok = 0;
    if ((obj->flags_1C & 2) || func_800A60D8(obj, &pos)) {
        ok = 1;
    }
    if (ok) {
        return func_800A5168(obj, &pos, flag);
    }
    obj->vtable->method_1C((u8 *)obj + obj->vtable->delta_18);
    return 0;
}
