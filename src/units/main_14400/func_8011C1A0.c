#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x; s32 y; } Pos;
typedef struct { Pos pos; u8 pad8[0x16]; u8 flags; } Obj;
extern u8 D_80142F1B;
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 message_id, ...);
void func_800EBE14(Obj *obj);
/* Item-effect slot +0x44 supplies self, actor and item; self and item are unused here. */
void func_8011C1A0(void *arg0, Obj *obj, void *item) {
    Pos pos;
    Pos *p;
    s32 ok = ((obj->flags >> 2) & 1) && !((D_80142F1B >> 2) & 1);
    if (ok) {
        p = &pos;
        pos.x = obj->pos.x;
        p->y = obj->pos.y;
        func_80049CB4(6);
        func_80049CB4(0x11D, p);
        func_80049CB4(7);
        func_80049CB4(0x129, 0xD);
        func_800EBE14(obj);
    } else {
        func_800498E4(0x225);
    }
}
