#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef short s16;
typedef struct { s16 delta; s16 index; void *fn; } VtblEntry;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef struct { u8 value; } Tmp800EF404;
typedef struct {
    char pad0[4];
    s32 active;
    char pad8[0x10];
    u16 unk18;
    char pad1A[0x16];
    VtblEntry *vtbl;
} Sub800EF404;
typedef struct {
    char pad0[0x84];
    Sub800EF404 sub;
    u16 unkB8;
    u8 lastLevel;
    u8 itemA;
    u8 itemB;
} Obj800EF404;
extern void *D_801476B8;

void *func_800A65E4(Tmp800EF404 *, Obj800EF404 *, void *);
void *func_800A6538(Tmp800EF404 *, Obj800EF404 *, void *);
void func_800A665C(Obj800EF404 *, Tmp800EF404 *);
char *func_800A3B20(void *);
u16 func_800E08B0(Obj800EF404 *);
u16 func_800E08F0(Obj800EF404 *);
char *func_800A8498(s32, s32);
char *func_800ACC30(u8);
s32 func_80049CB4(s32, ...);
void func_80049A04(u16, ...);
void func_80049BF0(s32);
void func_801F216C(u16, Obj800EF404 *);
s32 func_801F2BE8(Sub800EF404 *, s32);

static inline Sub800EF404 *toSub(Obj800EF404 *self) {
    return self != 0 ? &self->sub : 0;
}

s32 func_800EF404(Obj800EF404 *self, void *arg1) {
    if (self->sub.active != 0) {
        Tmp800EF404 tmp;
        u16 level;
        s32 base;
        s32 changed;
        char *message;

        if (self->unkB8 == 0) {
            return 1;
        }
        func_800A65E4(&tmp, self, arg1);
        func_800A665C(self, &tmp);
        level = 0;
        changed = 0;
        message = func_800A3B20(D_801476B8);
        {
            s32 off = ((D_80142F18.flags >> 2) & 1) ^ 1;
            if (off) {
                u16 hp = func_800E08B0(self);
                u16 maxHp = func_800E08F0(self);
                if (hp >= maxHp / 2) {
                    if (hp >= (maxHp * 3) / 4) {
                        base = 1;
                    } else {
                        base = 4;
                    }
                    if (self->itemA != 0) {
                        if (self->itemB != 0) {
                            level = base + 1;
                            changed = 1;
                            message = func_800A8498(self->itemA, self->itemB);
                        } else {
                            level = base | 2;
                            changed = 1;
                            message = func_800ACC30(self->itemA);
                        }
                    } else {
                        level = base;
                    }
                    if (changed && self->lastLevel != 0 && self->lastLevel < 7) {
                        s8 diff = level - self->lastLevel;
                        if (diff == 3 || diff == -3) {
                            level = base;
                            changed = 0;
                        }
                    }
                } else if (hp >= maxHp / 4) {
                    level = 7;
                } else {
                    level = 8;
                }
            }
        }
        func_80049CB4(0x128, 0x1A6);
        func_80049CB4(0xAA, self);
        func_80049A04((u16)(self->unkB8 + (u8)level), message);
        func_80049BF0(0);
        func_80049CB4(0xAB, self);
        if (!changed) {
            self->itemA = 0;
            self->itemB = 0;
        }
        self->lastLevel = level;
    } else if (toSub(self)->unk18 != 0) {
        Tmp800EF404 tmp2;
        func_801F216C(toSub(self)->unk18, self);
        if (func_801F2BE8(toSub(self), 1)) {
            func_800A6538(&tmp2, self, arg1);
            func_800A665C(self, &tmp2);
            return 1;
        }
    } else {
        Sub800EF404 *sub = &self->sub;
        VtblEntry *e = &self->sub.vtbl[1];
        ((void (*)(void *, s32))e->fn)((char *)sub + e->delta, 1);
    }
    return 1;
}
