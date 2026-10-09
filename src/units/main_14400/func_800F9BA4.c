#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u32 pad0 : 21;
    u32 bit10 : 1;
    u32 rest : 10;
} Flags800F9BA4;

typedef struct VTableA800F9BA4 VTableA800F9BA4;
typedef struct VTableB800F9BA4 VTableB800F9BA4;

typedef struct {
    char pad0[0xC];
    u32 unkC;
} Gauge800F9BA4;

typedef struct {
    char pad0[4];
    VTableA800F9BA4 *vtbl;
} Sub800F9BA4;

struct VTableA800F9BA4 {
    char pad0[0x20];
    short adjust20;
    s32 (*func24)(char *self);
    char pad28[0x38 - 0x28];
    short adjust38;
    Gauge800F9BA4 *(*func3C)(char *self, u32 arg1);
};

typedef struct {
    s32 unk0;
    s32 unk4;
} Pair800F9BA4;

typedef struct {
    char pad0[8];
    char unk8[0x1C];
    VTableB800F9BA4 *vtbl;
    char pad28[0x64 - 0x28];
    Pair800F9BA4 unk64;
    char pad6C[0x8C - 0x6C];
    Sub800F9BA4 *sub;
} Actor800F9BA4;

struct VTableB800F9BA4 {
    char pad0[0x90];
    short adjust90;
    s32 (*func94)(char *self, s32 arg1, s32 arg2, u8 arg3, s32 arg4);
};

typedef struct {
    char pad0[0x1E];
    u8 unk1E;
    char pad1F;
    Flags800F9BA4 flags;
    char pad24[0x72 - 0x24];
    u8 unk72;
    char pad73[0x84 - 0x73];
    u32 unk84;
} Target800F9BA4;

typedef struct {
    u8 kind;
    char pad1[2];
    u8 unk3;
} Item800F9BA4;

typedef struct {
    s32 unk0;
    s32 unk4;
} Pos800F9BA4;


/* Whole 4-byte roll clamp at D_8015690C (next object D_80156910); +2 is the maximum. */
typedef struct { u16 min; u16 max; } RollBounds;
extern RollBounds D_8015690C;
extern u32 D_80159F54[];
extern char D_80143094[];
extern u32 D_8013960C;

s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 id, ...);
void func_800497F0(s32 id, ...);
s32 func_800A58B8(Target800F9BA4 *target);
u32 func_800B1C6C(void *target);
s32 func_800E20CC(void *self);
s32 func_800A4754(Actor800F9BA4 *self, Actor800F9BA4 *other, char *pos);
Item800F9BA4 *func_800A6D18(Actor800F9BA4 *self);
char *func_800AE674(void *item);
void *func_800A6CC0(void *pos, void *self);
s32 func_800F9564(Actor800F9BA4 *self, Item800F9BA4 *item, Pos800F9BA4 *pos);
char *func_800A3B20(void *self);
s32 func_800A529C(void *self, s32 arg1);
s32 func_800F069C(Actor800F9BA4 *self);
s32 func_800AF950(char *arg0);
u16 func_800AB044(void);
u32 func_800F9504(Actor800F9BA4 *self);
s32 func_800E0F40(Actor800F9BA4 *self);
void func_800EB744(Target800F9BA4 *target, s32 amount);
void func_800E20F0(Target800F9BA4 *target);
Gauge800F9BA4 *func_801239D4(void);
s32 func_800AC670(Gauge800F9BA4 *gauge);
s32 func_800CD5C0(void *sub, void *gauge);
void func_800E3678(Target800F9BA4 *target, Actor800F9BA4 *self);
s32 func_8010E05C(Gauge800F9BA4 *gauge, s32 amount);
void func_800E03BC(Actor800F9BA4 *self, s32 arg1);

static inline s32 func_800F9BA4_flag(Flags800F9BA4 flags) {
    Flags800F9BA4 *p = &flags;

    return p->bit10;
}

s32 func_800F9BA4(Actor800F9BA4 *self, Target800F9BA4 *target) {
    s32 locked;
    s32 flag;
    s32 busy;
    s32 limited;
    s32 retry;
    u32 amount;
    u32 current;
    Gauge800F9BA4 *gauge;
    Pos800F9BA4 pos;
    s32 message;
    Pair800F9BA4 *pair;

    locked = D_80142F18.mode == 0x4F;
    if (locked) {
        func_80049CB4(0x104B, self);
        return 1;
    }
    flag = target == 0 || func_800A58B8(target) == 1 || (func_800B1C6C(target) & 0x4000);
    if (flag) {
        if (func_800E20CC(self)) {
            if (func_800A4754(self, self, self->unk8)) {
                Item800F9BA4 *item = func_800A6D18(self);
                s32 usable = item != 0 && item->kind == 0xE && item->unk3 == 2;

                if (usable) {
                    char *name = func_800AE674(item);

                    func_800A6CC0(&pos, self);
                    if (func_800F9564(self, item, &pos)) {
                        s32 who = func_80049CB4(0x104B, self);

                        func_800497F0(0x76, who, func_800A3B20(self), name);
                        func_800A529C(self, 1);
                        return 1;
                    }
                }
            }
            func_80049CB4(0x47, self, 1, 0);
            return 1;
        }
        return target == 0;
    }

    busy = (target->unk1E & 3) || (self->sub->vtbl->func24((char *)self->sub + self->sub->vtbl->adjust20) && func_800F069C(self)) || !func_800AF950(D_80143094);
    if (busy) {
fail:
        return 0;
    }
    amount = func_800AB044() * 3;
    if (amount > D_8015690C.max) {
        amount = D_8015690C.max;
    }
    limited = func_800F9BA4_flag(target->flags) || func_800F9504(self) >= D_80159F54[(u8)func_800E0F40(self) - 1];
    if (limited) {
        amount = 0;
    } else if ((target->unk1E >> 2) & 1) {
        if (amount > target->unk84) {
            amount = target->unk84;
        }
        func_800EB744(target, -amount);
    } else if (target->unk72 & 8) {
        if (amount > D_8015690C.max) {
            amount = D_8015690C.max;
        }
        target->unk72 &= ~8;
        func_800E20F0(target);
    } else {
        amount = 0;
    }

    gauge = self->sub->vtbl->func3C((char *)self->sub + self->sub->vtbl->adjust38, 0);
    if (gauge == 0 && amount != 0) {
        gauge = func_801239D4();
        if (func_800AC670(gauge)) {
            amount = 0;
        } else {
            gauge->unkC = 0;
            func_800CD5C0(self->sub, gauge);
        }
    }
    if (amount == 0) {
        retry = func_800E20CC(self) || func_800F9BA4_flag(target->flags);
        if (!retry) {
            goto fail;
        }
        func_80049CB4(0x47, self, 1, 0);
        func_800498E4(func_800F069C(self) ? 0x122 : 0x121);
        func_800E3678(target, self);
        return 1;
    }

    message = func_80049CB4(0x104B, self);
    func_8010E05C(gauge, amount);
    current = gauge->unkC;
    if (current > D_80159F54[(u8)func_800E0F40(self) - 1]) {
        gauge->unkC = D_80159F54[(u8)func_800E0F40(self) - 1];
    }
    pair = &self->unk64;
    pair->unk0 = 0;
    pair->unk4 = 0;
    if (func_800F069C(self)) {
        D_8013960C <<= 1;
        self->vtbl->func94((char *)self + self->vtbl->adjust90, 1, 0x11, 0, 0);
        func_800E03BC(self, 2);
        D_8013960C >>= 1;
    }
    func_80049CB4(0x132);
    func_800497F0(0x11F, message, func_800A3B20(self), amount);
    func_800E3678(target, self);
    pair->unk0 = 0;
    pair->unk4 = 0;
    func_800A529C(self, 1);
    return 1;
}
