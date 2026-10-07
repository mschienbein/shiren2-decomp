#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 kind; } Item80051860;
/* Text helpers read the handle at offset 0xC of this embedded object. */
typedef struct { u8 unknown_0[0xC]; s32 handle_C; } Text80051860;
typedef struct { s32 field_0; Text80051860 text; } Obj80051860;
extern char D_8014B578[];
extern char D_8014B57C[];
void func_80048870(Text80051860 *text, s32 flags);
s32 func_800AE598(Item80051860 *item);
s32 func_8010C38C(Item80051860 *item);
char *func_80048480(u16 id);
s32 func_8005EF30(char *dst, const char *fmt, ...);
s32 func_800ACEB4(Item80051860 *item);
void func_800487EC(Text80051860 *text, s32 style, s32 flags, char *str);
void func_80051860(Obj80051860 *obj, Item80051860 *item, s32 style) {
    char buf[0x40];
    s32 icon;
    if (item == 0) {
        return;
    }
    func_80048870(&obj->text, 0x78000000);
    if (func_800AE598(item)) {
        icon = 0x2F;
        if (item->kind == 3) {
            icon = 0x2E;
        }
        func_8005EF30(buf, D_8014B57C, func_8010C38C(item) ? func_80048480(0x2A0) : D_8014B578, icon);
        func_800487EC(&obj->text, style, 0, buf);
    } else {
        if (func_800ACEB4(item) != 2) {
            func_80048870(&obj->text, 0x38000000);
        }
        func_800487EC(&obj->text, style, 0, func_8010C38C(item) ? func_80048480(0x29F) : D_8014B578);
    }
}
