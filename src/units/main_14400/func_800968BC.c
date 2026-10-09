#include "common.h"
typedef unsigned char u8;
typedef struct { u8 kind; } Item800968BC;
/* Unit member at +0xF0 (vtable D_801544C0 installed at 0x800EB170): item-set slot +0x3C is
 * func_800CFC2C, void *(void *self, u32 index). */
typedef struct { u8 pad_00[0x38]; short adjust_38; short reserved_3A; void *(*get_3C)(void *self, u32 index); } VTable800968BC;
typedef struct { void *data_00; VTable800968BC *vtable_04; } Holder800968BC;
typedef struct { unsigned short command; unsigned short reserved_02; void *submenu_04; u32 field_08; } Choice800968BC;
typedef struct {
    u8 base_00[0x4C]; const void *vtable_4C; Choice800968BC *choices_50;
    s32 field_54; s32 state_58;
} Menu800968BC;
/* +0x64 is the complete 0x140-byte loader widget, the object func_80096140 constructs as its
 * sub64 (vtable D_80153078, +0x10C/+0x110 initialized) and func_800D86F4 allocates as a
 * 0x140-byte Loader: func_8009F680 writes +0x54, the status bytes +0x58..+0xF9, +0xFC, +0x120
 * and +0x128 before passing it to func_8009D910. */
typedef struct {
    u8 base_00[0x4C]; const void *vtable_4C; u8 pad_50[4]; s32 mode_54;
    u8 values_58[0xA2]; u8 pad_FA[2]; s32 count_FC; u8 pad_100[0xC];
    s32 field_10C; void *field_110; u8 pad_114[0xC]; unsigned short text_120;
    u8 pad_122[6]; s32 field_128; u8 pad_12C[0x14];
} Loader800968BC;
typedef struct { u8 pad_00[0x5C]; void *owner_5C; u8 pad_60[4]; Loader800968BC loader_64; } Obj800968BC;
/* Ten twelve-byte choice records occupy 801403F8..80140470. */
extern Choice800968BC D_801403F8[10];
/* Each complete menu is 0x5C zero-initialized bytes; the following choices are separate arrays. */
extern Menu800968BC D_80140390;
Menu800968BC D_80140278 = {{0}, 0, 0, 0, 0};
Choice800968BC D_801402D4[4] = {
    {0x279, 0, 0, 0x2E}, {0x28A, 0, 0, 0x47},
    {0x289, 0, 0, 0x3D}, {0x27A, 0, &D_80140390, 0x46}
};
Menu800968BC D_80140304 = {{0}, 0, 0, 0, 0};
Choice800968BC D_80140360[4] = {
    {0x27B, 0, 0, 0x2D}, {0x27D, 0, 0, 0x48},
    {0x27E, 0, 0, 0x49}, {0x27F, 0, 0, 0x4A}
};
Menu800968BC D_80140390 = {{0}, 0, 0, 0, 0};
typedef struct ExtendedMenu800968BC ExtendedMenu800968BC;
extern ExtendedMenu800968BC D_801404E0;
extern u8 D_80138D90[];
extern void *func_80096748(void);
extern Holder800968BC *func_800EBA54(void *owner);
extern void func_80097B90(void *obj, void *owner, void *holder, s32 mode, void *desc, s32 flag);
extern void func_8009F680(void *loader, s32 arg);
static __inline__ void prepare_menu(void *owner) {
    func_80097B90(&D_801404E0, owner, 0, 2, D_80138D90, 0);
}
static __inline__ void reset_menu(void) { D_80140278.state_58 = 0; }
void *func_800968BC(Obj800968BC *self, s32 choice) {
    switch (D_801403F8[choice].command) {
    case 0x273: return func_80096748();
    case 0x466: {
        Holder800968BC *holder = func_800EBA54(self->owner_5C);
        if (holder != 0) {
            Item800968BC *item = holder->vtable_04->get_3C((u8 *)holder + holder->vtable_04->adjust_38, 0);
            if (item != 0 && item->kind == 0x10) {
                prepare_menu(0);
                return &D_801404E0;
            }
        }
    }
    case 0x465:
        prepare_menu(func_800EBA54(self->owner_5C));
        return &D_801404E0;
    case 0x463: case 0x464:
        reset_menu();
        return &D_80140278;
    case 0x278: return &D_80140304;
    case 0x27A: return &D_80140390;
    case 0x27C:
        func_8009F680(&self->loader_64, 1);
        return &self->loader_64;
    default: return 0;
    }
}
