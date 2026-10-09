#include "common.h"
typedef unsigned char u8;
typedef struct { s32 field_0, field_4, field_8; } Triple;
typedef struct { s32 field_0, field_4, field_8, field_C; } Quad;
typedef struct { u8 pad_0[0x4C]; const void *field_4C; u8 pad_50[8]; u32 field_58; u8 pad_5C[0x24]; s32 field_80; Quad field_84; void *field_94; u8 field_98[0x10]; } Work;
typedef Work Obj;
typedef struct { u8 pad_0[0x84]; u32 field_84; } S;
typedef struct { u32 field_0; s32 field_4; } Balance;
extern S *D_801476B8;
extern u8 D_80152F40[], D_80151EC8[];
extern const unsigned char D_80151E38[144];
extern const Triple D_80139038;
extern const Quad D_80139044;
extern Obj *func_800953C0(Obj *o);
extern void func_8009F310(Work *, u32, s32, u32, Triple, s32, u32, Quad);
extern s32 func_800957C0(Work *work, void *out, s32 a2, void *a3, s32 a4);
extern void func_800EB744(S *s, s32 delta);
static inline u32 available_balance(S *actor) {
    return actor->field_84;
}
s32 func_800A1A64(Balance *balance) {
    u32 available = available_balance(D_801476B8);
    Work work;
    Work *self;
    s32 out[2];
    if (!available) return 0;
    self = &work;
    func_800953C0(self);
    self->field_4C = D_80152F40;
    work.field_80 = -1;
    work.field_94 = D_80151EC8;
    func_8009F310(self, available, 6, available, D_80139038, 0x1F1, balance->field_0, D_80139044);
    if ((func_800957C0(self, out, 1, 0, 0) ^ 1) != 0 || (balance->field_4 = self->field_58) == 0) {
        self->field_4C = D_80151E38;
        return 1;
    }
    if (balance->field_0 + balance->field_4 > 99999999U) {
        self->field_4C = D_80151E38;
        return 2;
    }
    balance->field_0 += balance->field_4;
    func_800EB744(D_801476B8, -balance->field_4);
    self->field_4C = D_80151E38;
    return 3;
}
