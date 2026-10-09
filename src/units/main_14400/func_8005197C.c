#include "common.h"

/* Item vtable entry 11: proven targets func_8010DCC4/func_8011146C return u16. */
typedef struct { unsigned char pad00[0x58]; short adjust58; short pad5A; unsigned short (*value5C)(void *); } ItemVTable;
typedef struct { unsigned char pad00[8]; ItemVTable *vtable08; signed char value0C; } Item;
typedef struct { unsigned char pad00[0xC]; s32 handle0C; } Text80051860;
typedef struct { s32 field00; Text80051860 text04; } Obj;
extern s32 func_800ACEB4(Item *item);
extern void func_80048870(Text80051860 *text, s32 flags);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);
extern s32 func_8005DFE8(unsigned char *str);
extern s32 func_8005EF30(char *dst, const char *fmt, ...);
extern void func_800487EC(Text80051860 *text, s32 style, s32 flags, char *str);
extern const char D_8014B590[];
extern const char D_8014B594[];

void func_8005197C(Obj *obj, Item *item, s32 slot)
{
    char number[0x40];
    char formatted[0x40];
    Text80051860 *text;
    s32 value = 0;
    s32 flags;
    if (item != 0) {
        if (func_800ACEB4(item) == 2) {
            value = item->vtable08->value5C((char *)item + item->vtable08->adjust58);
            flags = 0x78000000;
        } else {
            value = item->value0C;
            flags = 0x38000000;
        }
    } else {
        flags = 0x78000000;
    }
    text = &obj->text04;
    func_80048870(text, flags);
    func_8005EF08(number, D_8014B590, value);
    func_8005EF30(formatted, D_8014B594, 0x2F - func_8005DFE8((unsigned char *)number), number);
    func_800487EC(text, slot, 0, formatted);
}
