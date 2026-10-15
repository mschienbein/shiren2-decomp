/* Private combined s07 view (overlay_136dc0), including the four withheld misses.
 * Method tables follow the g++ 2.x non-thunk layout {s16 delta; s16 index; pfn}.
 * Shared-state extents follow fixes/s03; this header defines no storage. */
#ifndef S07_TYPES_H
#define S07_TYPES_H
#include "common.h"
#include "ovlb_shared_views.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Game800C9E00, Obj801F51E8, DestroyTable, OvlBItemDispatchView and
 * MenuPos have one shared definition in ovlb_shared_views.h. The native
 * virtual item view is deliberately separate from this manual dispatch view. */
/* List method table (canonical func_800CD468 Dispatch: count at +0x20/+0x24;
 * func_800A1DAC Vtbl800A1DACb: get at +0x38/+0x3C). count: lh/lw func_801EA204_13C124 0x801EA23C/
 * 0x801EA240 and equivalents; get: lh/lw 0x801EA26C/0x801EA270 and equivalents. */
struct ListEntry;
struct ListTable {
    u8 pad_00[0x20];
    s16 delta_20;
    s16 index_22;
    s32 (*count_24)(void *self);
    u8 pad_28[0x38 - 0x28];
    s16 delta_38;
    s16 index_3A;
    ListEntry *(*get_3C)(void *self, u32 index);
};
/* List: table pointer at +0x4 (lw 0x801EA234, 0x801EA394, 0x801EA470, 0x801EA534, 0x801EA58C,
 * 0x801EA674); canonical State {pool0; Dispatch *unk4}. */
struct List {
    void *unk_00;
    ListTable *table_04;
    /* Virtual dispatch through the method table, as g++ expands obj->count()/obj->get(i). */
    s32 count() { return table_04->count_24((u8 *)this + table_04->delta_20); }
    ListEntry *get(u32 index) { return table_04->get_3C((u8 *)this + table_04->delta_38, index); }
};
/* List entry: kind byte +0x0 (lbu 0x801EA294, 0x801EA3F4, 0x801EA4AC, 0x801EA57C, 0x801EA6B4),
 * id byte +0x1 (lbu 0x801EA280, 0x801EA3E8), nested list at +0xC (addiu 0x801EA220, 0x801EA588). */
struct ListEntry {
    u8 kind_00;
    u8 id_01;
    u8 pad_02[0xC - 0x2];
    List list_0C;
};

/* Unit returned by func_800A8CB0. +0x1E flag byte: lbu 0x801EA160/0x801EA1D8 (bit 2) and
 * 0x801EA318 (bits 2-3); canonical func_800A3934 reads (field1E >> 2) & 1. +0x24 method
 * table (canonical func_8010B5D0 Actor table_24, inventory slot +0x98/+0x9C): lw 0x801EA328,
 * lh/lw 0x801EA32C/0x801EA330. +0x84 word: lw 0x801EA1EC. +0x8C embedded list: addiu 0x801EA34C. */
struct OvlBInventoryUnitTable {
    u8 pad_00[0x98];
    s16 delta_98;
    s16 index_9A;
    List *(*inventory_9C)(void *self);
};
struct OvlBInventoryUnit {
    u8 pad_00[0x1E];
    u8 flags_1E;
    u8 pad_1F[0x24 - 0x1F];
    OvlBInventoryUnitTable *table_24;
    u8 pad_28[0x84 - 0x28];
    s32 unk_84;
    u8 pad_88[0x8C - 0x88];
    List list_8C;
};

/* Restore the original missing-consumer views without restoring its overlapping
 * +0x66 scalar. The s06 widget owner writes the complete word at +0x64
 * (801E8CBC/801E8CE8); 9D34 consumes its low signed halfword. */
struct Object80094C04 {
    u8 pad_00[4];
    DestroyTable *table_04;
};
struct Menu801E9D34 {
    u8 pad_00[0x4C];
    const void *table_4C;
    u8 pad_50[0x64 - 0x50];
    s32 result_64;
};
struct MenuSize { s32 rows; s32 width; MenuPos origin; };

extern "C" {
/* B .data byte bitmap 0x801F5248 (all zero in ROM): lbu/sb at %lo(0x5248) + (bit >> 3) in
 * func_801E9F70_13BE90 0x801E9F7C and func_801E9F90_13BEB0 0x801E9FB0/0x801E9FC4/0x801E9FE4/0x801E9FF8. */
extern u8 D_801F5248_147168[64];
/* Containing 32-byte table; func_801E9D0C_13BC2C reads element 2 (lbu 801E9D10).
 * Canonical func_800D1488 and the clear at 801EF6DC..801EF6E8 establish its extent. */
extern u8 D_801F5228_147148[32];
extern Obj801F51E8 *D_801F51E8_147108;
/* B .data word 0x801F5D04 (host-referenced): lw func_801EA024_13BF44 0x801EA028. */
extern s32 D_801F5D04_147C24;
/* Main rodata slot index table (canonical const u8 D_80154718[20]): lbu func_801E981C_13B73C 0x801E9850. */
extern const u8 D_80154718[20];
/* Main RNG state (canonical extern u8 D_80147620[]): address passed at 0x801E9C88. */
extern u8 D_80147620[];
/* Main player pointer (canonical extern void *D_801476B8): lw 0x801EA17C. */
extern void *D_801476B8;
extern const u8 D_8015488C[8];
extern signed char D_80140160[];
extern const unsigned char D_80151E38[144];
/* Main .data 0x80138F80..0x80138F98 (one 24-byte object in the accepted build,
 * between the pairs D_80138F78 and D_80138F98): three {x, y} menu origins
 * {2,13},{5,5},{6,6}. 801E9E08 loads element 1 (lw 0x80138F88/0x80138F8C). */
extern const MenuPos D_80138F80[3];
extern u8 D_801F5170_147090[];
extern u8 D_801F6F20_148E40[];
void func_800D1BB0(Game800C9E00 *game);
OvlBItemDispatchView *func_800AC244(u8 id);
s32 func_800AD714(OvlBItemDispatchView *item, MenuPos *pos);
void func_800AD7E0(OvlBItemDispatchView *item, MenuPos *pos, s32 notify);
s32 func_80049CB4(s32 id, ...);
Object80094C04 *func_80094C04(signed char *table, u8 value);
s32 func_800D8FF0(void *obj);
u16 func_8008387C(u8 *str);
u8 *func_8006A810(void *dst, s32 value, s32 count);
s32 func_800957C0(void *work, void *out, s32 a2, void *a3, s32 a4);
Menu801E9D34 *func_801E8B10_13AA30(Menu801E9D34 *menu, u8 *desc, MenuSize *size, MenuPos *pos, s32 flag, s32 a5);
void func_801EA810_13C730(OvlBItemDispatchView *item, s32 value);

Game800C9E00 *func_800C9E00(void);
u8 func_800C57CC(void *rng, s32 limit);
s32 func_800D1988(Game800C9E00 *game, s32 row);
OvlBInventoryUnit *func_800A8CB0(s32 cell);
s32 func_800A8974(void *unit);
void func_800EB744(OvlBInventoryUnit *unit, s32 amount);
/* Keep the ROM's explicit player argument; coordinated definition repair is owned by fixes/o2. */
void func_800485DC(void *player);
void func_800ACD34(ListEntry *entry);
void func_800CD468(List *list);
ListEntry *func_800CDA70(List *list, u8 id);
void func_800CD364(List *list, ListEntry *item);
OvlBItemDispatchView *func_801142D0(ListEntry *entry, u32 index);

s32 func_801E51A0_1370C0(void);
s32 func_801E51D8_1370F8(void);
void func_801E51E4_137104(void);
void func_801E7C6C_139B8C(s32);
u8 func_801E8CF4_13AC14(u8 index, u8 value);
s32 func_801E8D0C_13AC2C(u16 id, u16 selector);
void func_801E8F20_13AE40(s32);
u8 func_801E9F70_13BE90(s32 bit);
void func_801E9F90_13BEB0(s32 bit, u8 set);
u8 func_801E99C4_13B8E4(u8 slot, s32 alternate);
void func_801E9AFC_13BA1C(u8 slot, u8 value);
s32 func_801EA204_13C124(ListEntry *entry, s32 id, s32 byId);
List *func_801EA2D8_13C1F8(s32 id);
void func_801EB020_13CF40(s32, s32);
void func_801EC510_13E430(void);
}
#endif
