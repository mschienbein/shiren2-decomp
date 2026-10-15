#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
/* ODD_C: serialized entries have both word-copy and byte-field views.
   func_8009CA80 copies entry+4..+0x1B as six words (0x8009CB0C..38);
   this formatter reads bytes at +0x0B/+0x0C/+0x0F and text at +6. */
typedef struct {
    s32 field_00;
    union {
        struct {
            u8 field_04[2];
            u8 field_06[5];
            u8 field_0B;
            u8 field_0C;
            u8 field_0D[2];
            u8 field_0F;
            u8 field_10[0xC];
        } bytes;
        struct {
            s32 word_04;
            u8 bytes_08[3];
            u8 flags_0B;
            s32 words_0C[4];
        } words;
    } serial;
    s32 field_1C;
} Entry;
typedef struct {
    u8 field_00[0x50];
    Entry *field_50;
    s32 field_54;
    s32 field_58;
    u8 field_5C[0x10C];
    void *field_168;
} Object;
extern const u8 D_8015488C[8];
extern s32 D_80138E8C[];
typedef struct {
    u16 id;
    u16 pad2;
    void *field_4;
    s32 value;
} MenuItem;
/* Same 0x5C/0x10C widget views as func_8009C8FC.c and func_8009C920.c. */
typedef struct {
    u8 pad00[0x4C];
    const void *vtable;
    u8 pad50[0xC];
} Widget;
typedef struct {
    Widget base;
    u8 pad5C[0xC];
    s32 selection;
    const void *data;
    u8 pad70[0x9C];
} ExtendedWidget;
extern ExtendedWidget D_80141C24;
/* RECONSTRUCTION CHOICE (user ruling scratch/omp/rulings/RECONSTRUCTION-CHOICE-9C61C.md,
   "Allow documented choice", 2026-10-14). The six 12-byte records at 0x80141B80..0x80141BC8
   (ROM 0x114BC0..0x114C08) are declared as two three-item menus: [3] at 0x80141B80 and [3] at
   0x80141BA4. The original program does not fix this partition; [6], [1,2,1,2] and others give
   the same bytes. A whole-ROM scan found no other code, pointer table, loop, copy or image that
   touches the records; see opus-03 9c61c/scan_menu_refs.py and .json, and opus-03 result.json
   functions.func_8009C61C.extent_witness_search.
   Every observed access stays inside one array:
   - window starts: only &D_80141B80[0] (0x80141B80), &D_80141B80[1] (0x80141B8C),
     &D_80141BA4[0] (0x80141BA4) and &D_80141BA4[1] (0x80141BB0) (la a1, 0x8009C714..C7BC);
   - counts: the literal 1 or 2 (0x8009C6BC..C6C4), stored to D_80138E8C and options[0] for
     func_80097240. A window of 1 or 2 items from item 0 or 1 ends at item 2 of the same menu
     at the latest, for every flag value;
   - child pointers: only item 2 of each menu carries &D_80141C24 (words at 0x80141B9C and
     0x80141BC0, the scan's only data-word hits), with value 1. */
MenuItem D_80141B80[3] = {
    { 0x447 },
    { 0x444 },
    { 0x446, 0, &D_80141C24, 1 },
};
MenuItem D_80141BA4[3] = {
    { 0x444 },
    { 0x444 },
    { 0x445, 0, &D_80141C24, 1 },
};
Widget D_80141BC8 = { { 0 } };
ExtendedWidget D_80141C24 = { { { 0 } } };
extern u8 D_80138ECC[], D_80138EBC[], D_80138EB4[], D_80138EA4[], D_80138E9C[];
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_80097240(void *, void *, void *, void *);
extern char *func_80048480(u16 id);
extern void func_8009D610(void *, void *, void *, void *);
extern void func_8009D6E4(void *, s32);
extern u8 *func_80083F34(void *, s32, void *);
extern s32 func_800327C0(void *, const char *, ...);

/* ODD_C: the bitset accessor returns the selected mask, not a bool.
   Destructively updating bits invalidates the QI load's CSE equivalence. */
static inline u8 test_flags(const u8 *flags, u8 mask) {
    u8 bits;
    bits = *flags;
    bits &= mask;
    return bits;
}

void *func_8009C61C(Object *object, s32 index) {
    s32 options[2];
    char text[0x50];
    u8 label[0x10];
    void *menu;
    index /= 4;
    switch (object->field_58) {
        case 0:
            if (test_flags(&object->field_50[index].serial.bytes.field_0B, D_8015488C[1])) {
                u8 count;
                if (!test_flags(&object->field_50[index].serial.bytes.field_0B, D_8015488C[2])) count = 1;
                else count = 2;
                D_80138E8C[0] = count;
                func_8006A810((u8 *)options, 0, 8);
                options[0] = count;
                if (!test_flags(&object->field_50[index].serial.bytes.field_0B, D_8015488C[7])) {
                    func_80097240(&D_80141BC8, &D_80141B80[0], D_80138E8C, options);
                } else if (!test_flags(&object->field_50[index].serial.bytes.field_0B, D_8015488C[6])) {
                    func_80097240(&D_80141BC8, &D_80141B80[1], D_80138E8C, options);
                } else if (!test_flags(&object->field_50[index].serial.bytes.field_0C, D_8015488C[0])) {
                    func_80097240(&D_80141BC8, &D_80141BA4[0], D_80138E8C, options);
                } else if (!test_flags(&object->field_50[index].serial.bytes.field_0B, D_8015488C[2])) {
                    func_80097240(&D_80141BC8, &D_80141BA4[0], D_80138E8C, options);
                } else {
                    func_80097240(&D_80141BC8, &D_80141BA4[1], D_80138E8C, options);
                }
                if (object->field_50[index].serial.bytes.field_0F == 10) {
                    func_8009D610(&D_80141C24, func_80048480(0x256), D_80138ECC, D_80138EB4);
                } else {
                    func_8009D610(&D_80141C24, func_80048480(0x255), D_80138EBC, D_80138EB4);
                }
                func_8009D6E4(&D_80141C24, 0);
                menu = &D_80141BC8;
            } else {
                menu = 0;
            }
            break;
        case 3:
            *func_80083F34(object->field_50[index].serial.bytes.field_06, 4, label) = 0;
            func_800327C0(text, func_80048480(0x449), object->field_50[index].field_00 + 1, label);
            func_8009D610(object->field_5C, text, D_80138EA4, D_80138E9C);
            func_8009D6E4(object->field_5C, 0);
            menu = object->field_5C;
            break;
        case 2:
            menu = object->field_168;
            break;
        default:
            menu = 0;
            break;
    }
    return menu;
}
