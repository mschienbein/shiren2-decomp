#include "common.h"
#include "item_records.h"
#include "selection_records.h"

/* OpenAI gpt-6-astra / wave32 continuation of the Opus wave17 implementation;
 * wave34 (Anthropic Claude Opus) closes the last word with the byte-boolean
 * query idiom already canonical in sibling func_80097BE0.c.
 * wave34 r2: ODR repair (shared Item/ItemList/SelectionRecord contracts; private
 * views carry the unit suffix) and the receiver preconditions stated per Main's
 * F2 ruling (see openContents and case 6). */

/* g++ 2.8.1 item/command submenu of a 0x40C-byte menu panel.
 * D_80141A28 is proposed initialized storage here with its measured 0x70 extent.
 * Local symbol visibility permits the assembler's observed delay-slot fill;
 * it does not uniquely establish the historical translation-unit boundary.
 * Receiver preconditions are stated at their uses (Main F2 ruling, wave34). */

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef struct { s16 delta, index; s32 (*call)(void *, s32); } ValueSlot_80099478;
typedef struct { s16 delta, index; void *(*call)(void *); } ListSlot_80099478;
typedef struct { u8 pad00[0x98]; ListSlot_80099478 items; } UnitTable_80099478;
/* Unit status word at +0x20 (see func_800F532C); bit 23 is read from a snapshot. */
typedef struct { u32 bits; } Flags_80099478;
typedef struct { u8 pad00[0x20]; Flags_80099478 flags20; UnitTable_80099478 *table24; } Unit_80099478;
typedef struct { u8 pad00[0x68]; ValueSlot_80099478 value; } MenuTable_80099478;
typedef struct { u16 text, flags; void *submenu; s32 command; } Choice_80099478;
typedef struct { u8 bytes[0x80]; } Selector_80099478;
typedef struct { u8 bytes[0x10C]; } Dialog_80099478;
/* 0x78-byte item-detail UI child (filled by func_80096CE4); not an inventory list. */
typedef struct { u8 bytes[0x78]; } DetailView_80099478;
/* Ten rows precede the selector and the two embedded submenus in each panel. */
typedef struct {
    u8 pad00[0x4C]; MenuTable_80099478 *table4C;
    Choice_80099478 choices50[10]; Selector_80099478 selectorC8; Dialog_80099478 dialog148;
    DetailView_80099478 list254;
    s32 enabled2CC, amount2D0; u8 pad2D4[0x28]; s32 mode2FC;
    u8 pad300[0xB8]; s32 firstCount3B8; u8 pad3BC[0x50];
} Menu_80099478;
typedef struct { void *container; Item *item; } Pair_80099478;
typedef struct { u16 modes; s16 property; Choice_80099478 choice; } Descriptor_80099478;
/* Complete 0x70-byte head and its text object, as in func_8009A3C4. */
typedef struct { const void *vtbl; u8 unknown4[0xC]; s32 textHandle10; } PickerText_80099478;
typedef struct { u8 pad00[0x4C]; const void *vtbl; s32 found[2], count58; PickerText_80099478 text5C; } ItemPicker_80099478;
/* D_80147F98 is the mode field, not an independent object. */
typedef struct { void *records; void *methods; s8 mode; } Pool_80099478;
typedef struct Picker_80099478 Picker_80099478;
typedef struct NameEditor_80099478 NameEditor_80099478;

extern "C" {
extern SelectionRecord D_80142F18;
extern Unit_80099478 *D_801476B8;
extern Pool_80099478 D_80147F90;
extern Menu_80099478 D_801408EC, D_80140CF8, D_80141104;
extern Picker_80099478 D_80141994;
extern NameEditor_80099478 D_80141AD0;
}

/* Zero-filled .data, as the original toolchain emitted for objects with constructors
 * (same convention as D_80140CF8/D_80141104/D_80141510 in func_8009A514.c). */
ItemPicker_80099478 D_80141A28 = { { 0 } };

extern "C" {
extern const Descriptor_80099478 D_801522F4[];
extern s32 D_80138E0C[], D_80138E14[], D_80138E24[], D_80138E34[], D_80138E60[], D_80138E70[];
extern s32 D_80138E00[];
extern char D_801C3480[], D_801C3500[];
char *func_80048480(u16);
s32 func_8005EF08(char *, const char *, ...);
u8 *func_8006A810(u8 *, s32, s32);
void func_80096CE4(void *, void *, void *);
void func_80097B90(void *, void *, void *, s32, const void *, s32);
Pair_80099478 func_80098018(Menu_80099478 *, s32);
s32 func_8009A230(void *, Pair_80099478 *);
void func_8009A600(void *, u8 *, void *, void *, void *);
void func_8009A9D0(ItemPicker_80099478 *);
void func_8009ADE8(void *, s32, u8 *, s32);
void func_8009D610(void *, char *, void *, void *);
void func_8009D6E4(void *, s32);
void func_8009DFA0(void *);
s32 func_800A99D0(void);
s32 func_800AD468(u32);
Item *func_800E215C(void *);
void *func_800EBA54(void *);
s32 func_800EC630(void *, void *);
/* Pot content-list accessor: returns &pot->payload.pot.list (ROM 0x80114238
 * `addiu s0,a1,0xC`) after binding payload.pot.owner (+0x24, lw 0x18(s0) at
 * 0x80114240) to the pot through func_800D0054. */
ItemList *func_8011422C(Item *pot);
void *func_80099478(Menu_80099478 *self, s32 index);
}

/* Item query slot (ItemMethods +0x18/+0x1C), as in func_800C33FC. */
static inline s32 property(Item *item, s32 key) {
    ItemMethods *methods = item->field08;
    return methods->test1C((u8 *)item + methods->delta18, key);
}
static inline s32 value(Menu_80099478 *menu, s32 index) {
    ValueSlot_80099478 *slot = &menu->table4C->value;
    return slot->call((u8 *)menu + slot->delta, index);
}
static inline void *unit_items(Unit_80099478 *unit) {
    ListSlot_80099478 *slot = &unit->table24->items;
    return slot->call((u8 *)unit + slot->delta);
}
/* Fills the shared container submenu with the contents of a pot.
 * Original assumes the receiver is a kind-9 pot (constructed by func_80114060 /
 * func_801140C0, whose func_800CFF00 builds payload.pot.list); not checked in ROM.
 * The only gate is the virtual query 0x11 (0x80099874..0x80099888: lw 8(s4),
 * lh 0x18, lw 0x1C, jalr); every table found with func_80114CEC (answers 0x11)
 * is a pot table (D_8015D938 and the id 0xA7/A8/A9/AF/B1/B2 subclasses). The jump
 * table then reaches 0x80099BC4 (lbu 1(s4), id 0xAC test only) and calls
 * func_8011422C(s4) at 0x80099BD4 / 0x80099C5C with no kind or validity test. */
static inline void openContents(Item *pot, s32 mode, s32 *layout, s32 flag) {
    func_80097B90(&D_80141104, func_8011422C(pot), 0, mode, layout, flag);
}
/* ODD_C: Extract bit 23 from a real status snapshot in two destructive steps.
 * Compared with `return (flags->bits >> 23) & 1` (wave32 a1: 14 words),
 * this form (a7: 6 words) retains the snapshot in v1 and schedules its store
 * into the branch delay slot, matching 0x80099A04..0x80099A20. */
static inline s32 flag23(const Flags_80099478 *flags) {
    u32 bits = flags->bits;
    bits >>= 23;
    bits &= 1;
    return bits;
}

extern "C" void *func_80099478(Menu_80099478 *self, s32 index) {
    s32 item_index;
    if (!(self->mode2FC & 0xE80F)) return 0;
    if (value(self, index) == (s32)0x80000000) return 0;
    if (self->enabled2CC && self->amount2D0 > 0) {
        s32 selection[2];
        s32 allowed;
        func_8006A810((u8 *)selection, 0, sizeof(selection));
        self->choices50[0].submenu = 0;
        switch (self->mode2FC) {
        case 1:
            if (D_80147F90.mode >= 0) {
                self->choices50[0].text = 0x284;
                self->choices50[0].command = 0x28;
                selection[0] = 1;
            }
            break;
        case 4: case 0x2000:
            self->choices50[0].text = 0x285;
            self->choices50[0].command = 0x29;
            selection[0] = 1;
            if (D_80147F90.mode >= 0) {
                self->choices50[1].text = 0x284;
                self->choices50[1].command = 0x28;
                self->choices50[1].submenu = 0;
                selection[0] = 2;
            }
            break;
        case 0x20:
            self->choices50[0].text = 0x286;
            self->choices50[0].command = 0x2A;
            selection[0] = 1;
            break;
        default: selection[0] = 0; break;
        }
        allowed = 0;
        if (func_800A99D0()) {
            /* ODD_C: same idiom and same query as canonical func_80097BE0.c.
             * The comparison is held in a byte-sized boolean before widening;
             * combine folds the widening into the set and canonicalizes
             * (gtu x 0) to (ne x 0) = sne_si_zero, whose `sltu rd,$0,rs` has a
             * fixed $0. Assigned directly (or via an s32/bool helper, or a bool
             * local) it stays sgtu_si and reload_cse_regs replaces its zero
             * operand with s0 (known 0): `sltu s0,s0,v0`, 1 word (wave34 e03,
             * e10, e11). A u8-returning helper is byte-identical (e09). */
            u8 ok = D_80142F18.mode != 0x4F;
            allowed = ok;
        }
        if (allowed) {
            func_8005EF08(D_801C3480, func_80048480(0x28D), self->amount2D0);
            func_8009D610(&self->dialog148, D_801C3480, D_80138E60, D_80138E70);
            func_8009D6E4(&self->dialog148, 0);
            self->choices50[selection[0]].text = 0x287;
            self->choices50[selection[0]].submenu = &self->dialog148;
            self->choices50[selection[0]++].command = 0x2C;
        }
        if (selection[0] > 0) {
            D_80138E34[0] = selection[0];
            func_8009A600(&self->selectorC8, 0, self->choices50, D_80138E34, selection);
            return &self->selectorC8;
        }
        return 0;
    }
    item_index = value(self, index);
    if (item_index != (s32)0x80000000) {
        Pair_80099478 selected = func_80098018(self, item_index);
        /* Original assumes func_80098018 yields a live Item for a valid row index;
         * only null is checked in ROM (lw s4,0x1C(sp); beqz s4 at 0x80099700/04). */
        Item *item = selected.item;
        if (item) {
            Item *equipped = func_800E215C(D_801476B8);
            s32 mode = self->mode2FC;
            if (mode == 1 && item_index >= self->firstCount3B8) mode = 2;
            s32 count = 0;
            Choice_80099478 *choices = self->choices50;
            void *holder;
            if (equipped && property(equipped, 0x23)) holder = 0;
            else holder = func_800EBA54(D_801476B8);
            func_80097B90(&D_801408EC, unit_items(D_801476B8), holder, 0x20, D_80138E0C, 0);
            func_80097B90(&D_80140CF8, unit_items(D_801476B8), 0, 0x20, D_80138E0C, 0);
            for (s32 entry = 0; entry < 31; entry++) {
                const Descriptor_80099478 *descriptor = &D_801522F4[entry];
                s32 enabled = 0;
                if (!(descriptor->modes & mode)) continue;
                /* (f() ^ 1) != 0: the original keeps the xori before the branch (a plain
                 * f() != 1 compiles to bne against 1); canonical func_800BF944.c idiom. */
                if (descriptor->property == 0x25) enabled = 1;
                else if (property(item, descriptor->property)) enabled = 1;
                else if (descriptor->property < 0 && (property(item, -descriptor->property) ^ 1) != 0) enabled = 1;
                else if (descriptor->property == 0x15 && (func_800AD468(item->field01) ^ 1) != 0) enabled = 1;
                if (item->field01 == 0xCC && descriptor->choice.command == 0x11) enabled = 0;
                if (item->field00 == 9 && self->mode2FC == 0xC000 && descriptor->choice.command == 0x43) enabled = 0;
                if (descriptor->choice.command == 0x13 && equipped) {
                    if (equipped->field01 == 0xCC) enabled = 0;
                    else switch (equipped->field00) {
                    case 16: {
                        s32 blocked = 0;
                        if (func_800EC630(D_801476B8, equipped)) {
                            Flags_80099478 flags = D_801476B8->flags20;
                            blocked = !flag23(&flags);
                        }
                        if (blocked) enabled = 0;
                        break;
                    }
                    case 9: case 17:
                        if (self->mode2FC == 4) enabled = 0;
                        break;
                    case 15: case 19: case 20:
                        if (equipped->field01 != 0xF2) enabled = 0;
                        break;
                    }
                }
                if (descriptor->choice.command == 0x27) enabled = func_8009A230(self, &selected);
                Flags_80099478 flags = D_801476B8->flags20;
                if ((flag23(&flags) ^ 1) != 0) {
                    if (item->field00 == 16) {
                        s32 command = descriptor->choice.command;
                        if (command >= 0x11 && (command < 0x13 || (command == 0x1A && mode == 2))) enabled = 0;
                        if (descriptor->choice.text == 0x25D) enabled = 0;
                    } else if (item->field00 == 10) {
                        s32 command = descriptor->choice.command;
                        if (command >= 0x11 && (command < 0x13 || (command == 0x1A && mode == 2))) enabled = 0;
                    }
                }
                if (!enabled) continue;
                Choice_80099478 *choice = &choices[count];
                *choice = descriptor->choice;
                if (choice->text == 0x25D) {
                    func_80096CE4(&self->list254, item, D_80138E14);
                    choice->submenu = &self->list254;
                }
                if (choice->command == 0x13 && equipped && func_800EC630(D_801476B8, equipped)) {
                    choice->text = 0x26F;
                    choice->command = 0x4C;
                }
                if (choice->command == 0x27) {
                    char *format = func_80048480(0x28C);
                    func_8005EF08(D_801C3500, format, func_800AE674(item));
                    func_8009D610(&self->dialog148, D_801C3500, D_80138E60, D_80138E70);
                    func_8009D6E4(&self->dialog148, 0);
                    choice->submenu = &self->dialog148;
                }
                switch (descriptor->property) {
                case 17:
                    if (item->field01 == 0xAC) {
                        openContents(item, 0x10, D_80138E00, 0);
                    } else {
                        s32 child_mode;
                        s32 is_container = property(item, 0x13);
                        switch (self->mode2FC) {
                        case 0x800: child_mode = is_container ? 0x2000 : 0x4000; break;
                        case 0x8000: child_mode = 0xC000; break;
                        default: child_mode = is_container ? 4 : 8; break;
                        }
                        openContents(item, child_mode, D_80138E00, is_container);
                    }
                    choice->submenu = &D_80141104;
                    break;
                /* ODD_C: bind the submenu receiver before configuring and publishing it.
                 * These real per-case locals remove the extra invariant field-address
                 * quantity: c11 is 1 word off; repeated globals are 6 words off.
                 * Reversing the stores fixes allocation but grows text by 12 bytes. */
                /* Original assumes a property-6 receiver is a kind-2 consumable: every
                 * query override answering 6 (func_8011A3B8/A584/AB04/ACB0/B8DC) is
                 * installed after func_80112D20 by the id 0x19/0x1A/0x1D/0x1E/0x23
                 * constructors. Not checked in ROM: 0x80099CA0 is lbu 0xC(s4)
                 * straight from the jump table. Bit 0 is "identified" (func_800AE7BC). */
                case 6:
                    if (!(item->payload.consumable.flags0C & 1)) {
                        Menu_80099478 *submenu = &D_801408EC;
                        submenu->enabled2CC = 0;
                        choice->submenu = submenu;
                    } else choice->command = 0x18;
                    break;
                case 14: {
                    Menu_80099478 *submenu = &D_801408EC;
                    submenu->enabled2CC = 1;
                    choice->submenu = submenu;
                    break;
                }
                case 15: {
                    Menu_80099478 *submenu = &D_801408EC;
                    submenu->enabled2CC = 0;
                    choice->submenu = submenu;
                    break;
                }
                case 23:
                    func_8009DFA0(&D_80141994);
                    choice->submenu = &D_80141994;
                    break;
                /* Two descriptor properties open the same name editor; the original has
                 * two separate case bodies (two call sites). */
                case 22:
                    func_8009ADE8(&D_80141AD0, 4, 0, 1);
                    choice->submenu = &D_80141AD0;
                    break;
                case 21:
                    func_8009ADE8(&D_80141AD0, 4, 0, 1);
                    choice->submenu = &D_80141AD0;
                    break;
                case 0:
                    if (item->field00 == 6) {
                        if (!(item->flags02 & 4)) {
                            func_8009A9D0(&D_80141A28);
                            s32 found = D_80141A28.count58;
                            if (found >= 2) {
                                choice->submenu = &D_80141A28;
                                choice->command = 0x10;
                            }
                        }
                    }
                    break;
                }
                count++;
            }
            if (count > 0) {
                s32 output[2];
                D_80138E24[0] = count;
                func_8006A810((u8 *)output, 0, sizeof(output));
                output[0] = count;
                func_8009A600(&self->selectorC8, (u8 *)item, self->choices50, D_80138E24, output);
                return &self->selectorC8;
            }
        }
    }
    return 0;
}
