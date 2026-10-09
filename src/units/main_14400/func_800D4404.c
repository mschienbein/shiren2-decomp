#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct Rng Rng;
typedef struct Obj Obj;

/* Monster-kind info record returned by func_80044ECC. */
typedef struct { u16 base; u8 bonus; } KindInfo;

typedef struct {
    s32 field_00;
    s32 table_4;
} Request;

extern Rng D_80147620;
/* Zero-terminated kind lists (rodata). */
extern const u8 D_80156B38[];
extern const u8 D_80156B3C[];
extern const u8 D_80156B44[];
extern const u8 D_80156B4C[];
extern const u8 D_80156B54[];
extern const u8 D_80156B58[];
extern const u8 D_80156B60[];
extern const u8 D_80156B68[];

Obj *func_800AA63C(void);
void func_800F0130(Obj *obj);
void *func_800A83D0(s32 arg, u8 flags);
u8 func_800C57CC(void *rng, s32 limit);
KindInfo *func_80044ECC(u8 kind, u8 level);
s32 func_800C5844(void *rng, u8 base, u8 top);
void *func_800A8694(u8 kind, u8 variant, void *memory);

void *func_800D4404(Request *request) {
    const u8 *kinds;
    u8 count;
    u8 kind;
    s32 level;
    KindInfo *info;

    switch (request->table_4) {
    case 1:
        kinds = D_80156B38;
        break;
    case 2:
        kinds = D_80156B3C;
        break;
    case 3:
        kinds = D_80156B44;
        break;
    case 4:
        kinds = D_80156B4C;
        break;
    case 5:
        kinds = D_80156B54;
        break;
    case 6:
        kinds = D_80156B58;
        break;
    case 8:
        kinds = D_80156B60;
        break;
    case 9:
        kinds = D_80156B68;
        break;
    case 7:
        return func_800A83D0(0x5A, 2);
    case 0:
    default: {
        Obj *obj = func_800AA63C();
        if (obj != 0) {
            func_800F0130(obj);
        }
        return obj;
    }
    }
    for (count = 0; kinds[count] != 0; count++) {
    }
    kind = kinds[func_800C57CC(&D_80147620, (u8)(count - 1))];
    level = 1;
    info = func_80044ECC(kind, level);
    if (info != 0) {
        level = info->bonus;
    }
    return func_800A8694(kind, func_800C5844(&D_80147620, 1, level), 0);
}
