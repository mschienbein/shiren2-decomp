#include "common.h"
typedef unsigned char u8;
typedef struct { u8 high:5; u8 active:1; u8 low:2; } Flags;
typedef struct Actor { u8 pad_00[0x1E]; Flags flags_1E; } Actor;
typedef struct Item { u8 kind; u8 id; u8 pad_02[0xA]; u8 flags_0C; } Item;
extern u8 D_80148644[];
extern const u8 D_8015488C[8];
extern s32 func_800E1CC4(Actor *self, s32 kind);
extern void func_800498E4(s32 id, ...);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Actor *self);
extern char *func_800AE674(void *item);
extern void func_800ACDD8(void *item);
static __inline__ s32 is_active(Flags *flags) { return flags->active; }
static __inline__ void mark_known(u8 *bits, s32 bit) {
    bits[bit >> 3] |= D_8015488C[bit & 7];
}
static __inline__ void report(s32 id, Actor *target, Item *item) {
    char *name = func_800A3B20(target);
    char *item_name = func_800AE674(item);
    func_800498E4(id, name, item_name);
}
s32 func_80112F24(Item *item, Actor *target) {
    s32 extra;
    if (func_800E1CC4(target, 2)) {
        func_800498E4(0x71);
        return 0;
    }
    func_80049CB4(0x1131);
    extra = is_active(&target->flags_1E) && item->id != 0x2A;
    if (extra) {
        func_80049CB4(6);
        func_80049CB4(0x1134, 0);
        func_80049CB4(7);
    }
    func_80049CB4(6);
    func_80049CB4(0x1030, target, item);
    func_80049CB4(7);
    if (item->flags_0C & 1) {
        func_80049CB4(0x132);
        report(0xE3, target, item);
        if (is_active(&target->flags_1E)) func_80049CB4(0x136);
        return 0;
    }
    report(0x6B, target, item);
    func_800ACDD8(item);
    if (is_active(&target->flags_1E)) {
        mark_known(D_80148644, item->id - 0x17);
    }
    return 1;
}
