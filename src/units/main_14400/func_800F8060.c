#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct Inv800F8060 Inv800F8060;

/* Partial vtable views: count at +0x20, remove at +0x60, inventory at +0x98. */
typedef struct {
    u8 pad0[0x20];
    s16 countDelta;
    s16 countIndex;
    s32 (*count)(void *);
    u8 pad28[0x38];
    s16 removeDelta;
    s16 removeIndex;
    s32 (*remove)(void *, void *, s32);
} InvVTable800F8060;

struct Inv800F8060 {
    u8 pad0[4];
    InvVTable800F8060 *vtable;
};

typedef struct {
    u8 pad0[0x98];
    s16 inventoryDelta;
    s16 inventoryIndex;
    Inv800F8060 *(*inventory)(void *);
} ShopVTable800F8060;

typedef struct {
    u8 pad0[0x24];
    ShopVTable800F8060 *vtable;
} Shop800F8060;

typedef struct {
    u8 pad0[0x2CC];
    s32 unk2CC;
    u8 pad2D0[0x2EC - 0x2D0];
    void *unk2EC;
    u8 pad2F0[0x408 - 0x2F0];
    s16 unk408;
} List800F8060;

typedef struct {
    u8 pad0[0x4C];
    void *vtable;
    u8 pad50[0x68 - 0x50];
    s32 unk68;
    void *unk6C;
    u8 pad70[0x110 - 0x70];
} Menu800F8060;

typedef struct {
    Inv800F8060 *owner;
    void *item;
} Slot800F8060;

typedef struct {
    s32 selected;
    s32 pad4[7];
} Result800F8060;

extern Shop800F8060 *D_801476B8;
extern List800F8060 D_801404E0;
extern u8 D_80138FF8[];
extern u8 D_80152AE8[];
extern u8 D_80151EC8[];
extern u8 D_80151E38[];
extern u8 D_8014AB2C[];
extern u8 D_8014AB3C[];
s32 func_8009A3A4(void *item);

char *func_80048480(u16 textId);
void func_800498E4(s32 textId, ...);
void func_80049BF0(s32 arg);
s32 func_80049CB4(s32 id, ...);
s32 func_8005EF08(char *buf, const char *fmt, ...);
Menu800F8060 *func_800953C0(Menu800F8060 *menu);
s32 func_800957C0(void *menu, Result800F8060 *result, s32 a, void *b, s32 c);
void func_80097B90(List800F8060 *list, Inv800F8060 *inv, void *a, s32 b, void *c, s32 d);
void func_80099E50(List800F8060 *list);
s32 func_8009A038(List800F8060 *list);
void *func_8009A054(void *out, List800F8060 *list, s32 index);
void func_8009D610(Menu800F8060 *menu, char *buf, void *a, void *b);
char *func_800A3B20(void *self);
char *func_800AE674(void *item);
s32 func_800AEC2C(void *item);
void func_800D05A4(void *dst, Slot800F8060 *slot);
void *func_800D8FB0(u32 size);
u8 *func_800D9880(void *mem);

static inline void destroyMenu(Menu800F8060 *self) {
    self->vtable = D_80151E38;
}

static inline void initMenu(Menu800F8060 *self) {
    func_800953C0(self);
    self->vtable = D_80152AE8;
}

void *func_800F8060(void *self) {
    Result800F8060 result;
    Menu800F8060 menu;
    char buf[0xC8];
    Slot800F8060 slot;
    Inv800F8060 *inv;
    u8 *bundle;

    inv = D_801476B8->vtable->inventory((u8 *)D_801476B8 + D_801476B8->vtable->inventoryDelta);
    if (!inv->vtable->count((u8 *)inv + inv->vtable->countDelta)) {
        func_800498E4(0x1E0, func_800A3B20(self));
        return 0;
    }
    func_80097B90(&D_801404E0, inv, 0, 0x100, D_80138FF8, 0);
    D_801404E0.unk408 = 0x1E6;
    D_801404E0.unk2EC = func_8009A3A4;
    D_801404E0.unk2CC = 1;
    initMenu(&menu);
    menu.unk68 = -1;
    menu.unk6C = D_80151EC8;
retry:
    {
        s32 count;
        s32 total;
        s32 i;
        s32 failed;

        func_80099E50(&D_801404E0);
        failed = func_800957C0(&D_801404E0, &result, 1, 0, 0) != 1;
        if (failed) {
            func_800498E4(0x1DF, func_800A3B20(self));
            menu.vtable = D_80151E38;
            return 0;
        }
        bundle = func_800D9880(func_800D8FB0(0xC4));
        count = func_8009A038(&D_801404E0);
        total = 0;
        i = count;
        for (;;) {
            Inv800F8060 *owner;

            i--;
            if (i < 0) {
                break;
            }
            func_8009A054(&slot, &D_801404E0, i);
            owner = slot.owner;
            failed = owner->vtable->remove((u8 *)owner + owner->vtable->removeDelta, slot.item, 1) != 1;
            if (failed) {
                func_80049BF0(0);
                func_80049CB4(2);
                goto retry;
            }
            func_800D05A4(bundle + 0xB0, &slot);
            total += func_800AEC2C(slot.item);
        }
        if (count >= 2) {
            func_8005EF08(buf, func_80048480(0x1E4), count, total);
        } else {
            void *item;

            func_8009A054(&slot, &D_801404E0, 0);
            item = slot.item;
            func_8005EF08(buf, func_80048480(0x1E3), func_800AE674(item), total);
        }
        func_8009D610(&menu, buf, D_8014AB2C, D_8014AB3C);
        failed = func_800957C0(&menu, &result, 1, 0, 0) != 1;
        if (failed) {
            result.selected = 0;
        }
        if (!result.selected) {
            goto retry;
        }
    }
    func_800498E4(0x1DD, func_800A3B20(self));
    destroyMenu(&menu);
    return bundle;
}
