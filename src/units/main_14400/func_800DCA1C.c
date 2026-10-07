#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

/* field_8 carries the current actor object (D_801476B8) to the handler. */
typedef struct { s32 kind; s32 field_4; void *field_8; s32 field_C; s32 field_10; s32 field_14; s32 field_18; s32 field_1C; } Msg800DCA1C;
typedef struct { u8 pad0[0x38]; s16 delta; u8 pad3A[2]; s32 (*handler)(void *self, Msg800DCA1C *msg); } VTable800DCA1C;
typedef struct { u8 pad0[8]; VTable800DCA1C *vtable; } Target800DCA1C;
typedef struct { u8 pad0[0xC]; Target800DCA1C *target; } Obj800DCA1C;
extern void *D_801476B8;
s32 func_800DCA1C(Obj800DCA1C *obj) {
    Msg800DCA1C msg;
    Msg800DCA1C *p = &msg;
    Target800DCA1C *target;
    msg.kind = 0x16;
    p->field_4 = 0;
    p->field_18 = 1;
    p->field_8 = D_801476B8;
    target = obj->target;
    target->vtable->handler((u8 *)target + target->vtable->delta, p);
    return 0;
}
