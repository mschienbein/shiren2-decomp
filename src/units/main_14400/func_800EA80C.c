#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct Obj Obj;
/* Entity vtable (vptr at +0x24): +0x9C inventory getter, +0xB4 kind notification. */
typedef struct { s16 delta; s16 index; void *(*get)(void *self); } GetEntry800EA80C;
typedef struct { s16 delta; s16 index; void (*notify)(void *self, s32 kind); } KindEntry800EA80C;
typedef struct {
    u8 pad0[0x98];
    GetEntry800EA80C get_98;
    u8 padA0[0xB0 - 0xA0];
    KindEntry800EA80C kind_B0;
} ObjVtbl800EA80C;
struct Obj { u8 pad[0x24]; ObjVtbl800EA80C *vtbl; };
/* Inventory element vtable (vptr at +8): +0x3C message handler (root target func_800AF28C). */
typedef struct { s16 delta; s16 index; s32 (*handle)(void *self, void *msg); } MsgEntry800EA80C;
typedef struct { u8 pad0[0x38]; MsgEntry800EA80C msg_38; } ItemVtbl800EA80C;
typedef struct { u8 pad[8]; ItemVtbl800EA80C *vtbl; } Item;
typedef struct { s32 type; u8 pad[0x1C]; } Event;
typedef struct { u8 data[0x10]; } Iter;
typedef struct { u8 pad[0x10]; s32 x10; } Msg;
extern u32 D_8013960C;
extern void *func_800CEB20(Iter *, void *);
extern s32 func_800CEBA0(Iter *);
extern Item *func_800CEC68(Iter *);
void func_800EA80C(Obj *o, Msg *msg){
    Event ev;
    Iter it;
    void *list = o->vtbl->get_98.get((u8 *)o + o->vtbl->get_98.delta);
    s32 kind;
    if (list == 0) return;
    ev.type = 0x1E;
    D_8013960C <<= 1;
    func_800CEB20(&it, list);
    for (;;) {
        Item *item;
        if (!func_800CEBA0(&it)) break;
        item = func_800CEC68(&it);
        item->vtbl->msg_38.handle((u8 *)item + item->vtbl->msg_38.delta, &ev);
    }
    D_8013960C >>= 1;
    kind = msg->x10;
    if (kind < 6) {
        if (kind >= 2) o->vtbl->kind_B0.notify((u8 *)o + o->vtbl->kind_B0.delta, kind);
    }
}
