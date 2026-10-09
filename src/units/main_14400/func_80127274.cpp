#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* g++ vtable entry; entry 1 (+0x08) is the destructor, void (receiver, s32 flags). */
struct VEntry { short delta; short index; void (*destroy)(void *self, s32 flags); };
struct Unit;
struct Dir { u8 value; };
struct Obj { s32 x0; s32 x4; VEntry *vt8; s32 indexC; };
struct Msg { s32 kind; Unit *source; Unit *target; Dir dir; };

extern "C" {
/* Item parameter table entries (read-only data): range/damage pairs per item level. */
extern const u16 D_80156AA0, D_80156AA2, D_80156AA4, D_80156AA6, D_80156AA8, D_80156AAA;
s32 func_80049CB4(s32 id, ...);
void func_800A7204(void *target, void *source, void *direction, s32 range, s32 damage, s32 message_kind, s32 stop_on_terrain, s32 notify);
void func_800D3650(void *arg);
s32 func_80112B38(void *self, void *msg);
s32 func_80127274(Obj *o, Msg *m);
}

/* Message handler slot: s32 (receiver, event). */
s32 func_80127274(Obj *o, Msg *m)
{
    /*
     * Per-level effect values: g++ function-local statics with dynamic
     * initializers.  Each is filled on first use under its own guard word
     * (the guards are the .data words at 0x801487A0/0x801487A4, the tables
     * the .bss arrays at 0x801CA670/0x801CA676) and is read-only afterwards.
     */
    static const short damage_by_level[3] = { D_80156AA2, D_80156AA6, D_80156AAA };
    static const short range_by_level[3] = { D_80156AA0, D_80156AA4, D_80156AA8 };
    Dir dir;
    Unit *target, *source;
    s32 level;

    switch (m->kind) {
    case 0x12:
    case 0x13:
        dir = m->dir;
        target = m->target;
        func_80049CB4(0xA1, target);
        source = m->source;
        level = o->indexC - 1;
        func_800A7204(target, source, &dir, range_by_level[level], damage_by_level[level], 6, 0, 1);
        if (m->kind == 0x12) {
            func_800D3650(o);
            if (o != 0) o->vt8[1].destroy((char *)o + o->vt8[1].delta, 3);
        }
        return 1;
    }
    return func_80112B38(o, m);
}
