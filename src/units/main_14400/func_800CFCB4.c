#include "common.h"

typedef unsigned char u8;

typedef struct {
    u32 pad0 : 8;
    u32 bit23 : 1;
    u32 rest : 23;
} Flags800CFCB4;

typedef struct {
    char pad0[9];
    u8 unk9;
    char padA[0x20 - 0xA];
    Flags800CFCB4 flags;
} Player800CFCB4;

typedef struct VTable800CFCB4 VTable800CFCB4;

typedef struct {
    u8 kind;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    char pad4[4];
    VTable800CFCB4 *vtbl;
} Item800CFCB4;

struct VTable800CFCB4 {
    char pad0[0x18];
    short adjust18;
    s32 (*func1C)(char *self, s32 arg1);
};

typedef struct {
    char pad0[8];
    s32 position[2];
} Self800CFCB4;

extern Player800CFCB4 *D_801476B8;
char *func_800AE674(void *obj);
s32 func_800CD090(Self800CFCB4 *self, Item800CFCB4 *item);
s32 func_800E1CD4(Player800CFCB4 *player, s32 arg1);
void *func_800EB9FC(void *obj);
s32 func_800A251C(s32 *position, Player800CFCB4 *player);
void func_800498E4(s32 id, ...);

static inline s32 func_800CFCB4_flag(Flags800CFCB4 flags) {
    Flags800CFCB4 *p = &flags;

    return p->bit23;
}

s32 func_800CFCB4(Self800CFCB4 *self, Item800CFCB4 *item, s32 verbose) {
    char *name = func_800AE674(item);
    s32 blocked;
    s32 matches;
    s32 bad;

    if (func_800CD090(self, item) < 0) {
        return 0;
    }
    if (item->unk2 & 0x20) {
        if (verbose) {
            func_800498E4(0x82, name);
        }
        return 0;
    }
    if (item->vtbl->func1C((char *)item + item->vtbl->adjust18, 0x23) != 0) {
        if (verbose) {
            func_800498E4(0x10E);
        }
        return 0;
    }
    blocked = item->unk1 == 0xDB && func_800E1CD4(D_801476B8, 0x10);
    if (blocked) {
        if (verbose) {
            func_800498E4(0x10E);
        }
        return 0;
    }
    if (func_800EB9FC(D_801476B8)) {
        if (verbose) {
            func_800498E4(0x84, name);
        }
        return 0;
    }
    if (!func_800A251C(self->position, D_801476B8)) {
        return 1;
    }
    if (item->unk3 != (D_801476B8->unk9 & 0xF)) {
        return 0;
    }
    matches = item->kind == 0x10 || item->kind == 0xA;
    if (!matches) {
        return 1;
    }
    bad = func_800CFCB4_flag(D_801476B8->flags) != 1;
    if (bad) {
        if (verbose) {
            func_800498E4(0x10E);
        }
        return 0;
    }
    return 1;
}
