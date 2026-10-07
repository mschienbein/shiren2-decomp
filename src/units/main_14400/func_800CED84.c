#include "common.h"

typedef unsigned char u8;

typedef struct VTable800CED84 VTable800CED84;
typedef struct VTable800CED84b VTable800CED84b;

typedef struct {
    char pad0[2];
    u8 unk2;
} Item800CED84;

typedef struct {
    char pad0[0x1E];
    u8 unk1E;
    char pad1F[0x24 - 0x1F];
    VTable800CED84b *vtbl;
} Sub800CED84;

typedef struct {
    char pad0[4];
    VTable800CED84 *vtbl;
    char pad8[0x10 - 8];
    Sub800CED84 *sub;
} Obj800CED84;

struct VTable800CED84 {
    char pad0[0x38];
    short adjust38;
    Item800CED84 *(*func3C)(void *self, u32 index); /* item slot +0x3C get(self, index) */
};

struct VTable800CED84b {
    char pad0[0x60];
    short adjust60;
    void (*func64)(char *self);
};

extern u32 D_8013960C;
void func_800AE518(Item800CED84 *item, Sub800CED84 *sub, s32 arg2, s32 arg3);
/* Item slot +0x44 set(self, index, item): this override and the base func_800CE7D8 share it. */
void func_800CE7D8(Obj800CED84 *obj, s32 index, Item800CED84 *item);

void func_800CED84(Obj800CED84 *obj, s32 index, Item800CED84 *newItem) {
    Item800CED84 *item = obj->vtbl->func3C((char *)obj + obj->vtbl->adjust38, index);
    Sub800CED84 *sub;

    if (item != 0 && (item->unk2 & 4)) {
        D_8013960C <<= 1;
        func_800AE518(item, obj->sub, 0, 0);
        D_8013960C >>= 1;
    }
    func_800CE7D8(obj, index, newItem);
    sub = obj->sub;
    if (sub->unk1E & 0xC) {
        sub->vtbl->func64((char *)sub + sub->vtbl->adjust60);
    }
}
