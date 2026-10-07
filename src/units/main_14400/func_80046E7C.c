#include "common.h"
typedef short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { s32 a; s32 b; Pos pos; } Quad;
typedef struct { s16 delta; s16 index; void (*fn)(void *self); } SelfEntry;
typedef struct { s16 delta; s16 index; void (*fn)(void *self, Pos *pos); } PosEntry;
typedef struct {
    char pad0[0x10];
    SelfEntry slot2;
    char pad18[0x80 - 0x18];
    PosEntry slot16;
} VTable;
/* The dispatch sub-object at +0x10 is 0x10 bytes, including its vtable,
 * bound receiver and member-function descriptor. Only its consumers inspect it. */
typedef struct { s32 words[4]; } DispatchTarget;
typedef struct {
    char pad0[0x10];
    DispatchTarget unk10;
    s32 unk20;
    char pad24[4];
    Pos unk28;
    s16 unk30;
    char pad32[2];
    Pos unk34;
    char pad3C[8];
    s8 unk44;
    char pad45;
    s8 unk46;
    char pad47[5];
    VTable *vtbl;
} Obj;
u8 *func_8006A810(void *dst, s32 value, s32 count);
void func_800486A4(Obj *, Quad *, void *);
void func_800489B4(Obj *, s32);
void func_80046E7C(Obj *obj) {
    if (obj->unk46 != 0) {
        Quad copy;
        Quad q;
        func_8006A810(&q, 0, sizeof(q));
        q.a = obj->unk20 * 2;
        q.b = obj->unk30;
        q.pos = obj->unk28;
        copy = q;
        func_800486A4(obj, &copy, &obj->unk10);
        func_800489B4(obj, obj->unk44 != 0);
        obj->vtbl->slot16.fn((char *)obj + obj->vtbl->slot16.delta, &obj->unk34);
        obj->vtbl->slot2.fn((char *)obj + obj->vtbl->slot2.delta);
    }
    obj->unk46 = 0;
}
