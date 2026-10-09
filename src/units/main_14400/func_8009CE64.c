#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 count; s32 width; } Selection;
typedef struct { s32 columns; s32 width; s32 x; s32 y; } Layout;
/* 12-byte menu record: text id, argument, kind (D_80141D30-style entries). */
typedef struct { u16 id; u16 pad; void *arg; s32 kind; } MenuItem;

/* 0x5C-byte list menu built by func_80097240; selection copy at +0x3C. */
typedef struct {
    u8 pad_00[0x3C];
    Selection selection;
    u8 pad_44[0x18];
} ListMenu;

/* 0x16C-byte save list (func_8009C440): requested kind at +0x58, flag at +0x168. */
typedef struct {
    u8 pad_000[0x58];
    s32 kind;
    u8 pad_05C[0x10C];
    s32 flag_168;
} SaveList;

typedef struct { u8 bytes[0x20]; } SaveEntry;

/* Save menu shared with func_8009CA80/func_8009CC34/func_8009D068/func_8009D198. */
typedef struct {
    u8 pad_000[0x5C];
    MenuItem entries[9];
    s32 count;
    ListMenu confirm;
    SaveEntry saves[2];
    s32 saves_168;
    s32 empties_16C;
    SaveList list;
    MenuItem items[2];
    ListMenu empty_menu;
    u8 found_350[0x60];     /* func_8009D068 results: three 0x20-byte records */
    MenuItem slots[3];      /* func_8009D198 results */
} Menu;

extern MenuItem D_80141D9C[];
extern Layout D_80138F20;
extern Selection D_80138F30;
extern Layout D_80138F38;
extern Selection D_80138F48;
extern u8 D_80138F50[];
extern s32 D_80138F08[2];
/* D_80141DB4: the whole 0x6A4-byte save-slot group (see func_8009D2FC): a list menu
 * head padded to 0x64, then two 0x320-byte slot panels. */
typedef struct {
    ListMenu head;
    u8 pad_5C[0x64 - 0x5C];
    u8 panels[2][0x320];
} SlotGroup;
extern SlotGroup D_80141DB4;
typedef struct SubMenu SubMenu;
extern SubMenu D_801425F0;

extern void func_80097240(ListMenu *menu, MenuItem *items, Layout *layout, Selection *sel);
extern s32 func_80045D00(void);
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_8009EDE0(void *object);
extern s32 func_8009D068(Menu *menu);
extern void func_801E63C4(void);
extern s32 func_8009D198(Menu *menu, s32 first, s32 second);
/* Overlay 0x801E4EA0 helpers (overlay_1339f0): func_801E63C4 reads no arguments;
 * func_801E5F0C takes seven (a1..a3 are forwarded to func_801E5A30). */
extern void func_801E5F0C(ListMenu *menu, void *found, s32 count, u8 *layout, SaveEntry *saves, s32 saveCount, s32 *columns);

/* Opens the submenu for menu entry `index`; returns it, or 0 when there is none. */
void *func_8009CE64(Menu *menu, s32 index)
{
    s32 i;

    switch (menu->entries[index].kind) {
    case 2:
        for (i = 0; i < menu->empties_16C; i++) menu->items[i].arg = 0;
        menu->list.flag_168 = 0;
    case 0:
    case 3:
    case 5: {
        SaveList *list = &menu->list;

        list->kind = menu->entries[index].kind;
        return list;
    }
    case 1:
        for (i = 0; i < menu->empties_16C; i++) menu->items[i].arg = 0;
        return &menu->empty_menu;
    case 7: {
        Selection sel;

        func_80097240(&menu->confirm, D_80141D9C, &D_80138F20, &D_80138F30);
        if (func_80045D00() == 1) {
            func_8006A810((u8 *)&sel, 0, sizeof(sel));
        } else {
            func_8006A810((u8 *)&sel, 0, sizeof(sel));
            sel.count = 1;
        }
        menu->confirm.selection = sel;
        return &menu->confirm;
    }
    case 4:
        func_8009EDE0(&D_801425F0);
        return &D_801425F0;
    case 8: {
        s32 count = func_8009D068(menu);
        s32 n;

        if (count == -1) {
            func_801E63C4();
            return 0;
        }
        n = func_8009D198(menu, count, menu->saves_168);
        D_80138F38.columns = n;
        D_80138F48.count = n;
        func_80097240(&D_80141DB4.head, menu->slots, &D_80138F38, &D_80138F48);
        func_801E5F0C(&D_80141DB4.head, menu->found_350, count, D_80138F50, menu->saves, menu->saves_168, D_80138F08);
        return &D_80141DB4.head;
    }
    }
    return 0;
}
