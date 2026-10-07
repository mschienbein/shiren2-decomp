#include "common.h"
typedef struct { short delta; short index; void (*fn)(void *, s32); } VEntry;
typedef struct { char pad[0x8]; VEntry *vtbl; } Obj;
typedef struct { s32 unk0; s32 unk4; s32 pad8[2]; s32 unk10; s32 unk14; } Msg;
typedef struct { s32 a, b; } Pair;
char *func_800AE674(void *obj);
void func_800498E4(s32 message_id, ...);
s32 func_80049CB4(s32 id, ...);
void func_800D3650(Obj *);
s32 func_800AF28C(Obj *, Msg *);
static inline void Pair_copy(Pair *pp, Pair *src) { pp->a = src->a; pp->b = src->b; }
s32 func_80112B38(Obj *self, Msg *msg) {
    char *v = func_800AE674(self);
    if (msg->unk0 == 0xE) {
        Pair p;
        func_800498E4(msg->unk4 ? 0xA1 : 0xA0, v);
        Pair_copy(&p, (Pair *)&msg->unk10);
        func_80049CB4(0xC9, self, &p);
        func_800D3650(self);
        if (self) {
            VEntry *e = &self->vtbl[1];
            e->fn((char *)self + e->delta, 3);
        }
        return 1;
    }
    return func_800AF28C(self, msg);
}
