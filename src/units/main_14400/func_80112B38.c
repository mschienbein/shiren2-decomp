#include "common.h"
#include "item_records.h"
typedef struct { s32 unk0; s32 unk4; s32 pad8[2]; s32 unk10; s32 unk14; } Msg;
typedef struct { s32 a, b; } Pair;
void func_800498E4(s32 message_id, ...);
s32 func_80049CB4(s32 id, ...);
void func_800D3650(Item *);
s32 func_800AF28C(Item *, Msg *);
static inline void Pair_copy(Pair *pp, Pair *src) { pp->a = src->a; pp->b = src->b; }
s32 func_80112B38(Item *self, Msg *msg) {
    char *v = func_800AE674(self);
    if (msg->unk0 == 0xE) {
        Pair p;
        func_800498E4(msg->unk4 ? 0xA1 : 0xA0, v);
        Pair_copy(&p, (Pair *)&msg->unk10);
        func_80049CB4(0xC9, self, &p);
        func_800D3650(self);
        if (self) {
            ItemMethods *e = self->field08;
            e->method0C((char *)self + e->delta08, 3);
        }
        return 1;
    }
    return func_800AF28C(self, msg);
}
