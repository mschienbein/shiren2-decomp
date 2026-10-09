#include "common.h"
typedef unsigned char u8;
typedef struct { short delta; short index; void (*fn)(void *self, s32 mode, void *value); } VtEntry;
typedef struct { char pad[0x14]; s32 unk14; VtEntry *vt; } Obj;
typedef struct { u8 pad0[7]; u8 flags7; u8 pad8[0x14]; Obj *child1C; u8 tail20[0x10]; } Record;
extern Record D_80147680;
extern const char D_801541C0[];
extern u8 D_801476C3;
extern u8 D_80147620[];
extern signed char D_80140160[];
extern const u8 D_8015488C[8];
void func_800CA0A8(Obj *obj, s32 value);
void func_800CA4E8(Obj *obj, void *message);
void func_800C5BE8(void *source, Obj *destination);
void func_800C56D4(void *object);
void func_800C573C(void *object);
void func_800C9AF8(Obj *reader);
void func_800C9CC4(Obj *ctx);

/* Whole-object accessor: the flag byte and the mask table are read through
 * their containing objects. */
static inline s32 record_test(Record *record, const u8 *masks) { return record->flags7 & masks[1]; }

s32 func_800C993C(void)
{
    Obj *o = D_80147680.child1C;
    func_800CA0A8(o, 0x22);
    func_800CA4E8(o, (void *)D_801541C0);
    o->vt[5].fn((char *)o + o->vt[5].delta, 1, &D_801476C3);
    if (o->unk14) return 4;
    func_800C5BE8(D_80147620, o);
    if (o->unk14) return 4;
    func_800C56D4(D_80147620);
    func_800C9AF8(o);
    func_800C573C(D_80147620);
    if (o->unk14) return 2;
    if (D_80140160[6] != 1 && !record_test(&D_80147680, D_8015488C)) return 1;
    func_800C56D4(D_80147620);
    func_800C9CC4(o);
    func_800C573C(D_80147620);
    if (o->unk14) return 3;
    return 0;
}
