#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 type; void *source; void *target; } Msg;
extern u16 D_80156910;
void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
void func_800D3650(Item *);
s32 func_800AF28C(Item *, Msg *);
s32 func_8010E0A0(Item *s, Msg *m) {
    s32 v;
    switch (m->type) {
    case 0x12:
    case 0x13:
        v = 10000;
        if (s->field01 != 0xCD) {
            { u32 hp = s->payload.amount.amount0C; v = hp * D_80156910 / 100; }
            if (v >= 10000) v = 9999;
        }
        func_800A7B18(m->target, m->source, v, 6);
        if (m->type == 0x12) {
            func_800D3650(s);
            if (s) s->field08->method0C((u8 *)s + s->field08->delta08, 3);
        }
        return 1;
    }
    return func_800AF28C(s, m);
}
