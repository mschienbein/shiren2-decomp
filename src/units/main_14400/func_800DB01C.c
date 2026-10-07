#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

/* D_801476B8 is the current actor object (func_800DAD20 reads its words 0 and 4); kind-4
 * messages carry that object pointer at +4. The message is 32 bytes. */
typedef struct Actor800DB01C Actor800DB01C;
typedef struct { s32 kind; Actor800DB01C *actor; s32 pad8[6]; } Msg800DB01C;
typedef struct { u8 pad0[0x38]; s16 delta; u8 pad3A[2]; s32 (*handler)(void *self, Msg800DB01C *msg); } HandlerVTable800DB01C;
typedef struct { u8 pad0[8]; HandlerVTable800DB01C *vtable; } Handler800DB01C;
typedef struct { u8 pad0[0x60]; s16 delta; u8 pad62[2]; s32 (*check)(void *self, Handler800DB01C *h, s32 flag); } OwnerVTable800DB01C;
typedef struct { u8 pad0[4]; OwnerVTable800DB01C *vtable; } Owner800DB01C;
typedef struct { Owner800DB01C *owner; Handler800DB01C *handler; } Link800DB01C;
typedef struct { u8 pad0[8]; Link800DB01C link; } Obj800DB01C;
extern Actor800DB01C *D_801476B8;
void func_800DAD20(Obj800DB01C *obj, Handler800DB01C *h);
void func_800DAD80(Obj800DB01C *obj);
s32 func_800DB01C(Obj800DB01C *obj) {
    Msg800DB01C msg;
    Msg800DB01C *p;
    Handler800DB01C *h;
    s32 failed;
    Link800DB01C *link = &obj->link;

    failed = link->owner->vtable->check((u8 *)link->owner + link->owner->vtable->delta, link->handler, 1) ^ 1;
    if (failed) {
        return 0;
    }
    h = link->handler;
    func_800DAD20(obj, h);
    msg.kind = 4;
    p = &msg;
    p->actor = D_801476B8;
    (void)h->vtable->handler((u8 *)h + h->vtable->delta, p);
    func_800DAD80(obj);
    return 0;
}
