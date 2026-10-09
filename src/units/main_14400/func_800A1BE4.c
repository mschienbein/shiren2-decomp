#include "common.h"

typedef unsigned char u8;
typedef unsigned char VTable;
typedef struct { s32 x, y; } Pair;
typedef struct { s32 x, y, z; } Triple;
typedef struct { s32 x0, y0, x1, y1; } Rect;
typedef struct {
    u8 pad_00[0x4C];
    const VTable *field_4C;
    u8 pad_50[8];
    u32 field_58;
    u8 pad_5C[0x24];
    s32 field_80;
    u8 pad_84[0x10];
    const VTable *field_94;
    u8 pad_98[0x10];
} Work;
typedef struct { u32 amount, selected; } Entry;
typedef struct { u8 pad_00[0x84]; u32 field_84; } Player;
extern Player *D_801476B8;
extern const VTable D_80152F40[], D_80151EC8[], D_80151E38[];
extern const Triple D_80139038;
extern const Rect D_80139044;
extern Work *func_800953C0(Work *obj);
extern void func_8009F310(Work *obj, u32 maximum, s32 digits, u32 value, Triple layout, s32 title, u32 total, Rect bounds);
extern s32 func_800957C0(Work *work, void *out, s32 a2, void *a3, s32 a4);
extern void func_800EB744(Player *player, s32 delta);

static inline Work *init_work(Work *work) {
    func_800953C0(work);
    work->field_4C = D_80152F40;
    return work;
}

static inline s32 confirmed(Work *menu, Pair *out) {
    return func_800957C0(menu, out, 1, 0, 0) == 1;
}

s32 func_800A1BE4(Entry *entry) {
    Work work;
    Pair result;
    Work *menu;
    /* ODD_C: total is scratch for gold, the clamped remainder, then the late
     * sum. Reusing it keeps the original v1 load and s3 gold copy. Separate
     * gold/direct-clamp forms, a direct late sum, and retaining only the
     * clamp reuse each miss 98 words in the reviewer's measured probes. */
    u32 total = D_801476B8->field_84;
    u32 gold = total;
    u32 count = entry->amount;
    u32 maximum;
    if (count == 0) {
        return 0;
    }
    maximum = count;
    if (maximum + total > 999999U) {
        total = 999999U - total;
        maximum = total;
    }
    if (maximum != 0) {
        menu = init_work(&work);
        work.field_80 = -1;
        work.field_94 = D_80151EC8;
        func_8009F310(menu, maximum, 6, maximum, D_80139038, 0x1F2, entry->amount, D_80139044);
        if (!confirmed(menu, &result) || (entry->selected = count = menu->field_58) == 0) {
            menu->field_4C = D_80151E38;
            return 1;
        }
        {
            u32 limit = 99999999U;
            total = gold + count;
            if (total <= limit) {
                func_800EB744(D_801476B8, count);
                entry->amount -= entry->selected;
                menu->field_4C = D_80151E38;
                return 3;
            }
        }
        menu->field_4C = D_80151E38;
    }
    return 2;
}
