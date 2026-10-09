#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct {
    u8 pad0[0x8];
    s16 off_get;
    u8 padA[2];
    s32 (*get)(void *self);
    u8 pad10[0x8];
    s16 off_set;
    u8 pad1A[2];
    void (*set)(void *self, s32 value);
} VTable800EB1A0;
/* Whole 0x18-byte derived collection at actor+0xCC (constructed by func_800CEC90). */
typedef struct {
    void *pool_00;
    VTable800EB1A0 *vtable;
    u8 *entries_08;
    u8 state_0C[4];
    void *owner_10;
    u16 text_14;
    u8 pad_16[2];
} Sub800EB1A0;
typedef struct { s32 x; s32 y; } Pos800EB1A0;
typedef struct {
    Pos800EB1A0 pos;
    u8 pad8[0x17];
    u8 field_1F;
    u8 pad20[0x8];
    s16 field_28;
    s16 field_2A;
    s16 field_2C;
    s16 field_2E;
    s16 field_30;
    u8 field_32;
    u8 pad33[0x3F];
    u8 field_72;
    u8 pad73[0x5];
    s32 field_78;
    s32 field_7C;
    u8 pad80[0x4];
    s32 field_84;
    s32 field_88;
    u8 pad8C[0x4];
    s32 field_90;
    u8 field_94;
    u8 pad95[0x37];
    Sub800EB1A0 sub;
    s16 field_E4;
    u8 padE6[0x2];
    s32 field_E8;
    s32 field_EC;
    u8 padF0[0x14];
    void *field_104;
    u8 field_108;
} Obj800EB1A0;
extern u32 D_8013960C;
extern s32 D_80148360;
/* Whole shared start-position object, including both coordinates. */
extern Pos800EB1A0 D_801C9F10;
void func_800E039C(Obj800EB1A0 *obj, s32 value);
void func_800EB35C(Obj800EB1A0 *obj, u8 seconds);
void func_800EB330(Obj800EB1A0 *obj, u8 value);
void func_800ECA18(Obj800EB1A0 *obj);
void func_800CD468(Sub800EB1A0 *sub);
static inline void pos_clear(Pos800EB1A0 *p) { p->x = 0; p->y = 0; }
void func_800EB1A0(Obj800EB1A0 *obj) {
    Sub800EB1A0 *sub;
    obj->field_28 = 0xF;
    obj->field_2A = 0xF;
    obj->field_2C = 8;
    obj->field_2E = 8;
    obj->field_30 = 0;
    obj->field_78 = 0;
    D_8013960C <<= 1;
    obj->field_1F = 0x17;
    obj->field_32 = 1;
    func_800E039C(obj, 1);
    obj->field_7C = 0;
    obj->field_84 = 0;
    func_800EB35C(obj, 100);
    func_800EB330(obj, 100);
    obj->field_90 = 0;
    obj->field_94 = 0;
    func_800ECA18(obj);
    sub = &obj->sub;
    sub->vtable->set((u8 *)sub + sub->vtable->off_set, sub->vtable->get((u8 *)sub + sub->vtable->off_get));
    func_800CD468(sub);
    obj->field_108 = 0;
    obj->field_72 &= ~0x10;
    obj->field_104 = 0;
    obj->field_E4 = 0;
    obj->field_E8 = 0;
    obj->field_EC = obj->field_88;
    if (D_80148360 == 0) {
        D_80148360 = 1;
        pos_clear(&D_801C9F10);
    }
    obj->pos = D_801C9F10;
    D_8013960C >>= 1;
}
