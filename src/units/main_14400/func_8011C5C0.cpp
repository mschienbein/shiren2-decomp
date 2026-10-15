#include "map_position_iterator.h"

/* This function's views: a room rectangle of two MapPos corners, the room walk iterator
   (current/start/end), the handler's flag word (bit 23 active), the 0x20-byte event with
   MapDir at +0xC and MapPos at +0x10, and the floor object (kind +0, vtable +8). */
struct Flags { unsigned pad : 8; unsigned active : 1; unsigned rest : 23; };
struct Msg { s32 type; char pad[8]; MapDir dir; char pad2[3]; MapPos pos; char pad3[8]; };
struct VEntry { short delta; short index; s32 (*fn)(void *receiver, void *event); };
struct VTable { char pad[0x38]; VEntry e; };
struct Obj_8011C5C0 { unsigned char kind; char pad[7]; VTable *vtbl; };
/* The handler's second argument (this function is the +0x44 slot of the kind-2 item table at
   0x8015EAC0): a record with an s32 position at +0 and a flag word at +0x20. Not the 0x30-byte
   item record of item_records.h, whose +0..+3 are bytes. */
struct PosFlagsView_8011C5C0 { MapPos pos; char pad[0x18]; Flags flags; };
extern "C" {
Obj_8011C5C0 *func_800B4D80(MapPos *);
s32 func_80049CB4(s32, ...);
void func_800498E4(s32, ...);
}
static inline MapPos rect_lo(MapRect *r) { return r->min; }
static inline MapPos rect_hi(MapRect *r) { return r->max; }
static inline s32 flags_active(Flags *f) { return f->active; }
static inline MapPos *copy_pos(MapPos *out, const MapPos *in) {
    out->x = in->x;
    out->y = in->y;
    return out;
}
static inline void init_message(Msg *msg, const MapPos &at, const MapDir &direction) {
    msg->type = 0x15;
    msg->pos = at;
    msg->dir = direction;
}
extern "C" void func_8011C5C0(void *self /* unused */, PosFlagsView_8011C5C0 *item, void *target /* unused callback input */) {
    MapPos p;
    Msg msg;
    MapPositionIterator it;
    Flags f;
    bool warned;
    f = item->flags;
    if (flags_active(&f)) {
        {
            MapRect r = func_800B3024(copy_pos(&p, &item->pos));
            it.start = rect_lo(&r);
            it.cur = it.start;
            it.end = rect_hi(&r);
        }
        warned = false;
        for (;;) {
            MapPos at;
            Obj_8011C5C0 *o;
            s32 more = it.cur.x <= it.end.x;
            if (!more) break;
            func_800A3610(&at, &it);
            o = func_800B4D80(&at);
            if (o == 0 || o->kind != 0x10) continue;
            if (!warned) {
                func_80049CB4(0x11D, &p);
                warned = true;
            }
            init_message(&msg, at, MapDir(6));
            o->vtbl->e.fn((char *)o + o->vtbl->e.delta, &msg);
        }
    } else {
        func_80049CB4(0x132);
        func_800498E4(0x223);
    }
}
