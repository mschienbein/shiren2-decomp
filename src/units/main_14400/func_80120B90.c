#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct Message {
    void *field_0;
    s32 field_4;
    s32 field_8;
    s16 field_C;
    u16 field_E;
    u8 field_10;
} Message;
typedef struct { u8 data[0x14]; } Loc;
typedef struct { s16 delta; s16 index; void *(*fn)(void *self); } VEntry;
typedef struct { u8 pad[0x24]; VEntry *vt; } Actor;
typedef struct { u8 pad[0x28]; u8 f_28; } Obj;
extern u8 D_80147620[];
extern u8 D_80156A15;
extern u8 D_80156A17;
s32 func_800E8DC8(void *unit, s16 value);
s32 func_800E33D0(void *obj, Message message, u8 rays, u8 range);
void *func_800A6CC0(void *out_position, void *obj);
u32 func_800B1C6C(Loc *loc);
s32 func_80049CB4(s32 id, ...);
s32 func_800C587C(void *rng, u8 chance);
char *func_800AC990(void *obj);
void func_800498E4(s32 message_id, ...);
void func_801146A4(void *obj, void *pos, u16 message_id);
void func_800D3650(Obj *o);
void func_800CD364(void *p, Obj *o);
static inline void query_init(Message *q, s16 v) {
    q->field_C = v;
    q->field_10 = 10;
    q->field_E = 0;
    q->field_8 = 0;
}
void func_80120B90(Obj *obj, Actor *actor) {
    s32 hit = 0;
    Message q;
    s32 found;
    query_init(&q, func_800E8DC8(actor, 0));
    {
        Message tmp = q;
        found = func_800E33D0(actor, tmp, 1, 1);
    }
    if (found) {
        hit = func_800C587C(D_80147620, D_80156A15);
    } else {
        Loc loc;
        func_800A6CC0(&loc, actor);
        if (func_800B1C6C(&loc) & 0x4000) {
            func_80049CB4(0x128, 0xA9);
            hit = func_800C587C(D_80147620, D_80156A17);
        }
    }
    if (hit && obj->f_28) {
        func_800498E4(0xB8, func_800AC990(obj));
        func_80049CB4(6);
        func_801146A4(obj, actor, 0);
        func_80049CB4(7);
        func_80049CB4(0x85, actor, obj);
        func_800D3650(obj);
        func_800CD364(actor->vt[19].fn((u8 *)actor + actor->vt[19].delta), obj);
    }
}
