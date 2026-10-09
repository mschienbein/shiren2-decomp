#include "common.h"

struct Pos {
    s32 x, y;
    Pos() {}
    Pos(const Pos &p) : x(p.x), y(p.y) {}
};
struct Rect { Pos lo, hi; };
struct Iter { Pos cur, start, end; };
struct Dir {
    unsigned char v;
    Dir() {}
    Dir(unsigned char value) : v(value) {}
};
struct Flags { unsigned pad : 8; unsigned active : 1; unsigned rest : 23; };
struct Msg { s32 type; char pad[8]; Dir dir; char pad2[3]; Pos pos; char pad3[8]; };
struct VEntry { short delta; short index; s32 (*fn)(void *receiver, void *event); };
struct VTable { char pad[0x38]; VEntry e; };
struct Obj { unsigned char kind; char pad[7]; VTable *vtbl; };
struct Item { Pos pos; char pad[0x18]; Flags flags; };
extern "C" {
/* Returns its rectangle by value through the hidden result pointer. */
Rect func_800B3024(Pos *);
Pos *func_800A3610(Pos *, Iter *);
Obj *func_800B4D80(Pos *);
s32 func_80049CB4(s32, ...);
void func_800498E4(s32, ...);
}
static inline Pos rect_lo(Rect *r) { return r->lo; }
static inline Pos rect_hi(Rect *r) { return r->hi; }
static inline s32 flags_active(Flags *f) { return f->active; }
static inline Pos *copy_pos(Pos *out, const Pos *in) {
    out->x = in->x;
    out->y = in->y;
    return out;
}
static inline void init_message(Msg *msg, const Pos &at, const Dir &direction) {
    msg->type = 0x15;
    msg->pos = at;
    msg->dir = direction;
}
extern "C" void func_8011C5C0(void *self /* unused */, Item *item, void *target /* unused callback input */) {
    Pos p;
    Msg msg;
    Iter it;
    Flags f;
    bool warned;
    f = item->flags;
    if (flags_active(&f)) {
        {
            Rect r = func_800B3024(copy_pos(&p, &item->pos));
            it.start = rect_lo(&r);
            it.cur = it.start;
            it.end = rect_hi(&r);
        }
        warned = false;
        for (;;) {
            Pos at;
            Obj *o;
            s32 more = it.cur.x <= it.end.x;
            if (!more) break;
            func_800A3610(&at, &it);
            o = func_800B4D80(&at);
            if (o == 0 || o->kind != 0x10) continue;
            if (!warned) {
                func_80049CB4(0x11D, &p);
                warned = true;
            }
            init_message(&msg, at, Dir(6));
            o->vtbl->e.fn((char *)o + o->vtbl->e.delta, &msg);
        }
    } else {
        func_80049CB4(0x132);
        func_800498E4(0x223);
    }
}
