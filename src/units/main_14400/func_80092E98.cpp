#include "common.h"

/* g++ 2.8.1 unit: the unit command menu is a local object whose inline destructor
 * runs on every return (func_80096B84 without the delete). */

typedef unsigned char u8;
typedef short s16;

/* Container/item link produced by the struct-returning queries. */
typedef struct { void *container; void *item; } Pair;

typedef struct MenuBase { char pad0[0x4C]; const void *vtbl; } MenuBase;
/* 0x40C-byte menu panels (D_801404E0, D_801408EC, D_80141104). */
typedef struct MenuPanel {
    MenuBase base;
    char pad50[0xC8 - 0x50];
    MenuBase second;
    char pad118[0x128 - 0x118];
    char unk128[0x194 - 0x128];
    const void *unk194;
    char pad198[0x2A0 - 0x198];
    const void *unk2A0;
    char pad2A4[0x2CC - 0x2A4];
    s32 field_2CC, field_2D0;
    char pad2D4[0x40C - 0x2D4];
} MenuPanel;

typedef struct { void *entries[8]; s32 count; } Selection;
/* Field (main) menu owner: selection history at +0x64, chosen item at +0x88. */
typedef struct { u8 pad_0[0x64]; Selection selection_64; void *item_88; } Object;

/* Unit command menu built by func_80096140 (0x1A4 bytes): own vtable at +0x4C, the
 * list member at +0x64 with its vtable at +0xB0. */
typedef struct Unit Unit;
extern "C" const unsigned char D_80151E38[144];
extern "C" s32 D_80151F30[];
extern "C" char D_80151350[], D_80149F30[];
struct Menu;
extern "C" Menu *func_80096140(Menu *menu, Unit *unit);
/* Widget base: method table at +0x4C; its destructor reinstalls the root table. */
struct Widget {
    char pad0[0x4C];
    const void *vtable;
    ~Widget() { vtable = D_80151E38; }
};

/* 0x140-byte list member (vtable at +0x4C). */
struct ListMenu : Widget {
    char pad50[0x140 - 0x50];
};

/* Unit command menu built by func_80096140 (0x1A4 bytes). */
struct Menu : Widget {
    char pad50[0x64 - 0x50];
    ListMenu sub64;
    Menu() {}
    ~Menu() { vtable = D_80151F30; }
};

/* Cursor menu (root table D_80151350, vptr at +0xC). */
struct MenuCursor {
    s32 index;
    void *records;
    s32 done;
    const void *vtable_0C;
    MenuCursor() { vtable_0C = D_80151350; }
    ~MenuCursor() { vtable_0C = D_80151350; }
};

/* Cursor menu over the field options (method table D_80149F30). */
struct OptionCursor : MenuCursor {
    OptionCursor() { records = 0; vtable_0C = D_80149F30; }
};

typedef struct { s16 delta, index; void *(*call)(void *self, u32 index); } ItemSlot;
typedef struct { u8 pad_00[0x38]; ItemSlot item; } ItemSetVtbl;
typedef struct { s32 unk_00; ItemSetVtbl *vtbl; } ItemSet;

typedef struct Command Command;

extern "C" Unit *D_801476B8;
extern "C" MenuPanel D_801404E0, D_801408EC, D_80141104;


extern "C" void func_80048380(void);
extern "C" s32 func_800957C0(void *object, void *output, s32 modal, void *history, s32 event);
extern "C" void *func_800D0180(void *pair);
extern "C" void *func_800D8FB0(u32 size);
extern "C" Command *func_800DFD80(void *obj);
extern "C" Command *func_800DFED0(void *obj);
extern "C" Command *func_800DF740(void *obj);
extern "C" Command *func_800DFE40(void *obj);
extern "C" Command *func_800DEEC0(void *obj);
extern "C" Command *func_800DA010(void *obj);
extern "C" Command *func_800D9F20(void *obj);
extern "C" Command *func_800DFC80(void *obj);
extern "C" Command *func_800DD970(void *obj, void *context);
extern "C" Command *func_800DB7DC(void *obj, Pair *source, Pair *target);
/* Original call sites explicitly supply these otherwise-unused receivers. */
extern "C" void func_800480A0(MenuCursor *unused_receiver);
extern "C" s32 func_800918B0(MenuCursor *self);
extern "C" void func_800480EC(void *unused_receiver);
extern "C" void func_80094DAC(Object *unused_receiver);
extern "C" void func_80094E10(Object *unused_receiver);
extern "C" void func_80094F30(Object *unused_receiver);
extern "C" Pair func_80097F20(MenuPanel *menu);
extern "C" s32 func_800D0248(void *object);
extern "C" void func_80095CC0(Selection *self, s32 count);
extern "C" void *func_80093944(void *unused, s32 type, Pair *ctx, void *source);
extern "C" void *func_80093530(void *unused, s32 type, Pair *ctx);
extern "C" ItemSet *func_800EBA54(Unit *unit);
extern "C" void *func_800D0190(void *output, void *helper, void *item);

extern "C" void *func_80092E98(Object *self)
{
    Unit *unit = D_801476B8;
    s32 choices[8];
    s32 i;
    s32 selected;

    func_80048380();
    for (i = 7; i >= 0; i--) choices[i] = 0;
    Menu menu;
    Menu *active = &menu;
    func_80096140(active, unit);
    selected = func_800957C0(active, choices, 1, &self->selection_64, 0) == 1;
    if (!selected) return 0;
    Pair ctx;
    MenuPanel *work;
    func_800D0180(&ctx);
    switch (choices[0]) {
    case 0x44:
        return func_800DFD80(func_800D8FB0(8));
    case 0x35:
        return func_800DFED0(func_800D8FB0(8));
    case 0x45:
        switch (choices[1]) {
        case 0x2E:
            return func_800DF740(func_800D8FB0(0xC));
        case 0x47: {
            OptionCursor cursor;

            func_800480A0(&cursor);
            func_800918B0(&cursor);
            func_800480EC(&cursor);
            break;
        }
        case 0x3D:
            return func_800DFE40(func_800D8FB0(8));
        case 0x46:
            switch (choices[2]) {
            case 0x2D:
                return func_800DEEC0(func_800D8FB0(8));
            case 0x48:
                func_80094DAC(self);
                return 0;
            case 0x49:
                func_80094E10(self);
                return 0;
            case 0x4A:
                func_80094F30(self);
                return 0;
            }
            return 0;
        }
        return 0;
    case 0x34:
        return func_800DA010(func_800D8FB0(8));
    case 0x41:
        return func_800D9F20(func_800D8FB0(8));
    case 0x30:
        if (choices[1] != 0x30) return 0;
        return func_800DFC80(func_800D8FB0(8));
    case 0x42:
        if (choices[1] == 0x30) return func_800DFC80(func_800D8FB0(8));
    case 0x1B:
    case 0x40:
        if (choices[2] == 0x43) {
            work = &D_80141104;
            ctx = func_80097F20(work);
            selected = func_800D0248(&ctx) == 1;
            if (!selected) return 0;
            choices[2] = choices[4];
            choices[3] = choices[5];
        } else {
            work = &D_801404E0;
            ctx = func_80097F20(work);
        }
        if (choices[2] == 0x27) {
            if (choices[3] == 0) return 0;
            return func_800DD970(func_800D8FB0(0x10), &ctx);
        }
        if (choices[2] == 0x15) {
            func_80095CC0(&self->selection_64, 2);
            self->item_88 = ctx.item;
            work = &D_801408EC;
            s32 available = 0;
            if (work->field_2CC != 0) available = work->field_2D0 > 0;
            if (available) choices[2] = 0x2A;
        }
        switch (choices[2]) {
        case 0x28:
        case 0x29:
        case 0x2A:
            return func_80093944(self, choices[2], &ctx, work);
        case 0x2C:
            if (choices[3] == 0) return 0;
            return func_80093944(self, 0x2C, &ctx, work);
        case 0x4C: {
            ItemSet *set = func_800EBA54(D_801476B8);
            Pair source;

            func_800D0190(&source, set, set->vtbl->item.call((u8 *)set + set->vtbl->item.delta, 0));
            return func_800DB7DC(func_800D8FB0(0x18), &source, &ctx);
        }
        }
        selected = func_800D0248(&ctx) == 1;
        if (!selected) return 0;
        return func_80093530(self, choices[2], &ctx);
    }
    return 0;
}
