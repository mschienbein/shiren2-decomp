#include "common.h"

typedef struct {
    char pad0[0x4C];
    const void *vtbl;
} Base8009D2FC;

typedef struct {
    Base8009D2FC base;
    char pad50[0x68 - 0x50];
    s32 unk68;
    void *unk6C;
} Item8009D2FC;

typedef struct {
    Base8009D2FC outer;
    char pad50[0x5C - 0x50];
    Item8009D2FC item1;
    char pad1[0x17C - 0x5C - sizeof(Item8009D2FC)];
    Item8009D2FC item2;
    char pad2[0x288 - 0x17C - sizeof(Item8009D2FC)];
    Base8009D2FC tail;
    char pad3[0x320 - 0x288 - sizeof(Base8009D2FC)];
} Pair8009D2FC;

typedef struct {
    Base8009D2FC head;
    char pad50[0x64 - 0x50];
    Pair8009D2FC pairs[2];
} Group8009D2FC;

extern Group8009D2FC D_80141DB4;
/* 0x90-byte derived menu vtables in overlay_1339f0 (0x801E80A0..0x801E812F, 0x801E8130..0x801E81BF). */
extern const unsigned char D_801E80A0[144];
extern const unsigned char D_80152968[152];
extern const unsigned char D_80152AE8[144];
extern char D_80151EC8[];
extern const unsigned char D_801E8130[144];
extern const unsigned char D_801521D0[144];

void *func_800953C0(void *obj);
void func_8009D2FC(void) {
    Base8009D2FC *outer0;
    Item8009D2FC *itemA0;
    Item8009D2FC *itemB0;
    Base8009D2FC *tail0;
    Base8009D2FC *outer1;
    Item8009D2FC *itemA1;
    Item8009D2FC *itemB1;
    Base8009D2FC *tail1;

    func_800953C0(&D_80141DB4.head);
    D_80141DB4.head.vtbl = D_801E80A0;

    outer0 = &D_80141DB4.pairs[0].outer;
    func_800953C0(outer0);
    outer0->vtbl = D_80152968;
    itemA0 = &D_80141DB4.pairs[0].item1;
    func_800953C0(itemA0);
    itemA0->base.vtbl = D_80152AE8;
    D_80141DB4.pairs[0].item1.unk68 = -1;
    D_80141DB4.pairs[0].item1.unk6C = D_80151EC8;
    outer0->vtbl = D_801E8130;
    itemB0 = &D_80141DB4.pairs[0].item2;
    func_800953C0(itemB0);
    itemB0->base.vtbl = D_80152AE8;
    D_80141DB4.pairs[0].item2.unk68 = -1;
    D_80141DB4.pairs[0].item2.unk6C = D_80151EC8;
    tail0 = &D_80141DB4.pairs[0].tail;
    func_800953C0(tail0);
    tail0->vtbl = D_801521D0;

    outer1 = &D_80141DB4.pairs[1].outer;
    func_800953C0(outer1);
    outer1->vtbl = D_80152968;
    itemA1 = &D_80141DB4.pairs[1].item1;
    func_800953C0(itemA1);
    itemA1->base.vtbl = D_80152AE8;
    D_80141DB4.pairs[1].item1.unk68 = -1;
    D_80141DB4.pairs[1].item1.unk6C = D_80151EC8;
    outer1->vtbl = D_801E8130;
    itemB1 = &D_80141DB4.pairs[1].item2;
    func_800953C0(itemB1);
    itemB1->base.vtbl = D_80152AE8;
    D_80141DB4.pairs[1].item2.unk68 = -1;
    D_80141DB4.pairs[1].item2.unk6C = D_80151EC8;
    tail1 = &D_80141DB4.pairs[1].tail;
    func_800953C0(tail1);
    tail1->vtbl = D_801521D0;
}
