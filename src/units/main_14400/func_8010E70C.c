#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 data[0x18]; } Buf;
typedef struct { s16 delta; s16 index; void (*fn)(void *self, s32 flags); } VEntry;
typedef struct { s32 f_0; s32 f_4; VEntry *vt; } Obj;
s32 func_8010E5FC(void *arg0, void *unit);
void func_800C4F34(Buf *b, u16 v);
s32 func_800A6EE0(void *obj);
void *func_800C4DA0(void *self, void *owner, void *action, u16 flags);
void func_800C4864(Buf *b, s32 message_id, void *position);
void func_800D3650(Obj *obj);
/* direction and target are supplied by the callers (func_80123390, func_80123490) but unused here. */
void func_8010E70C(Obj *obj, void *unit, void *direction, void *target, void *position) {
    Buf tmp;
    Buf out;
    func_800C4F34(&tmp, func_8010E5FC(obj, unit));
    func_800C4DA0(&out, unit, &tmp, func_800A6EE0(unit));
    func_800C4864(&out, 0, position);
    func_800D3650(obj);
    if (obj) obj->vt[1].fn((u8 *)obj + obj->vt[1].delta, 3);
}
