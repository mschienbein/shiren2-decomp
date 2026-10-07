#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s16 delta;
    s16 index;
    void *(*func)(void *self);
} VtableEntry;

typedef struct {
    s16 delta;
    s16 index;
    void *(*func)(void *self, u32 index);
} IndexedVtableEntry;

typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self, s32 action, s32 kind, u8 mode, s32 flags);
} UnitActionEntry;

typedef struct {
    char pad0[0x38];
    IndexedVtableEntry entry38;
} SourceVtable;

typedef struct {
    s32 unk0;
    SourceVtable *vtable;
} Source80096140;

typedef struct {
    char pad0[0x90];
    UnitActionEntry entry90;
    VtableEntry entry98;
} UnitVtable;

typedef struct {
    char pad0[0x24];
    UnitVtable *vtable;
    char pad28[0x104 - 0x28];
    s32 unk104;
} Unit80096140;

typedef struct {
    u8 kind;
} Info80096140;

typedef struct {
    u16 id;
    u8 width;
    u8 pad3;
    s32 value;
} MenuDef80096140;

typedef struct {
    u16 id;
    char pad2[0x8 - 0x2];
    s32 value;
} MenuItem80096140;

typedef struct {
    s32 x;
    s32 y;
} Pair80096140;

typedef struct {
    s32 columns;
    s32 width;
    Pair80096140 origin;
} Layout80096140;

typedef struct {
    s32 count;
    s32 width;
} Selection80096140;

typedef struct {
    char pad0[0x4C];
    void *vtable;
} MenuSub80096140;

typedef struct {
    char pad0[0x4C];
    void *vtable;
    char pad50[0x5C - 0x50];
    Unit80096140 *unit;
    char pad60[0x64 - 0x60];
    MenuSub80096140 sub64;
    char padB4[0x170 - 0xB4];
    s32 unk170;
    void *unk174;
} Menu80096140;

extern char D_80151F30[];
extern char D_80153078[];
extern char D_80151EC8[];
extern MenuDef80096140 D_80151EDC[10];
extern MenuItem80096140 D_801403F8[];
extern u8 D_801403EC[];
extern u8 D_80140165;
extern u8 D_80142F24;
extern u8 D_80142F1B;
extern s16 D_80140260;
extern Pair80096140 D_80138D88;

extern void *func_800953C0(void *obj);
extern Source80096140 *func_800EBA54(Unit80096140 *unit);
extern s32 func_800E4454(Unit80096140 *unit);
extern s32 func_800EC630(Unit80096140 *unit, Info80096140 *info);
extern s32 func_801E9D0C(void);
extern s32 func_800A99D0(void);
extern s32 func_800CDA70(void *item, s32 kind);
extern void func_80097240(Menu80096140 *menu, MenuItem80096140 *items, Layout80096140 *layout,
                          Selection80096140 *sel);

Menu80096140 *func_80096140(Menu80096140 *menu, Unit80096140 *unit) {
    Source80096140 *source;
    MenuSub80096140 *sub;
    Info80096140 *info;
    Layout80096140 layout;
    Selection80096140 sel;
    u8 kind;
    s32 count;
    s32 i;
    s32 enabled;
    Pair80096140 *origin;

    func_800953C0(menu);
    menu->vtable = D_80151F30;
    sub = &menu->sub64;
    func_800953C0(sub);
    sub->vtable = D_80153078;
    menu->unk170 = -1;
    menu->unk174 = D_80151EC8;
    menu->unit = unit;
    kind = 0;
    info = 0;
    source = func_800EBA54(unit);
    if (source != 0) {
        info = source->vtable->entry38.func((char *)source + source->vtable->entry38.delta, 0);
        if (info != 0) {
            kind = info->kind;
        }
    }
    count = 0;
    i = 0;
    while (1) {
        if (i >= 10) {
            break;
        }
        enabled = 1;
        switch (D_80151EDC[i].id) {
            case 0x273: {
                s32 busy = 0;

                if (menu->unit->unk104 != 0 || func_800E4454(menu->unit) != 0 ||
                    menu->unit->vtable->entry90.func((char *)menu->unit + menu->unit->vtable->entry90.delta, 2, 9,
                                                     busy, 0) != 0) {
                    busy = 1;
                }
                if (busy) {
                    enabled = 0;
                }
                break;
            }
            case 0x276:
                if (menu->unit->unk104 == 0) {
                    enabled = 0;
                }
                break;
            case 0x277: {
                s32 idle = menu->unit->unk104 == 0 && func_800E4454(menu->unit) == 0;

                if (idle) {
                    enabled = 0;
                }
                break;
            }
            case 0x466: {
                s32 locked = (D_80140165 & 1) ^ 1;

                if (locked) {
                    enabled = 0;
                } else if (kind == 15) {
                    enabled = 0;
                } else {
                    s32 busy = 0;

                    if (menu->unit->unk104 != 0 || func_800E4454(menu->unit) != 0 ||
                        menu->unit->vtable->entry90.func((char *)menu->unit + menu->unit->vtable->entry90.delta, 2,
                                                         9, busy, 0) != 0) {
                        busy = 1;
                    }
                    if (busy) {
                        enabled = 0;
                    } else if (kind == 16 && func_800EC630(menu->unit, info) != 0) {
                        enabled = 0;
                    }
                }
                break;
            }
            case 0x463:
                if (kind != 15) {
                    enabled = 0;
                } else if (menu->unit->unk104 != 0) {
                    enabled = 0;
                }
                break;
            case 0x465:
                if (kind != 16) {
                    enabled = 0;
                } else {
                    Unit80096140 *u = menu->unit;
                    s32 busy = 0;

                    if (u->unk104 != 0 ||
                        u->vtable->entry90.func((char *)u + u->vtable->entry90.delta, 2, 9, busy, 0) != 0) {
                        busy = 1;
                    }
                    if (busy) {
                        enabled = 0;
                    } else {
                        s32 locked = func_800EC630(menu->unit, info) ^ 1;

                        if (locked) {
                            enabled = 0;
                        }
                    }
                }
                break;
            case 0x274:
                {
                    s32 locked = func_801E9D0C() ^ 1;

                    if (locked) {
                        enabled = 0;
                    }
                }
                break;
            case 0x275:
            case 0x27C:
                if (func_800A99D0() != 0) {
                    enabled = 0;
                } else {
                    s32 missing = 0;

                    if (D_80142F24 != 7) {
                        Unit80096140 *u = menu->unit;

                        missing = func_800CDA70(u->vtable->entry98.func((char *)u + u->vtable->entry98.delta), 0xAC) == 0;
                    }
                    if (missing) {
                        enabled = 0;
                    }
                }
                break;
            case 0x278:
                break;
            default:
                enabled = 0;
                break;
        }
        if (enabled) {
            s32 id = D_80151EDC[i].id;
            s32 value = D_80151EDC[i].value;
            u8 width = D_80151EDC[i].width;

            D_801403F8[count].id = id;
            D_801403F8[count].value = value;
            D_801403EC[count] = width;
            if (id == 0x463 && (D_80142F1B & 3) == 3) {
                D_801403F8[count].id = 0x464;
            }
            count++;
        }
        i++;
    }
    switch (D_80142F1B & 3) {
        case 2:
            D_80140260 = 0x434;
            break;
        case 1:
            D_80140260 = 0x433;
            break;
        default:
            D_80140260 = 0x435;
            break;
    }
    origin = &D_80138D88;
    layout.origin.x = origin->x;
    layout.origin.y = origin->y;
    if (count >= 4) {
        s32 right = 0;
        s32 j = 0;
        s32 half;
        s32 left = 0;

        D_801403F8[count].id = 0;
        D_801403F8[count].value = 0x80000000;
        half = (count + 1) / 2;
        for (; j < half; j++) {
            if (left < D_801403EC[j]) {
                left = D_801403EC[j];
            }
        }
        for (j = half; j < count; j++) {
            if (right < D_801403EC[j]) {
                right = D_801403EC[j];
            }
        }
        layout.columns = half;
        layout.width = (left + right) * 2;
        sel.count = count;
        sel.width = left;
    } else {
        s32 j;
        s32 width;

        width = 0;
        for (j = 0; j < count; j++) {
            if (width < D_801403EC[j]) {
                width = D_801403EC[j];
            }
        }
        layout.columns = count;
        layout.width = width * 2;
        sel.count = count;
        sel.width = width;
    }
    func_80097240(menu, D_801403F8, &layout, &sel);
    return menu;
}
