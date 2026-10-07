#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 field_0; u8 field_1; u8 pad2[0xA]; u8 field_C; u8 field_D; } Item;
typedef struct { u8 pad0[0xE4]; u16 field_E4; } Obj;
extern u16 D_8014767C;
char *func_800AE674(void *obj);
s32 func_800ACEB4(Item *item);
s32 func_800A692C(Obj *obj, s32 kind);
void func_800ACD34(Item *item);
void func_800498E4(s32 message_id, ...);
s32 func_801F27DC(Item *item);
void func_800EC0F4(Obj *obj, Item *item) {
    s32 fresh;
    s32 hit;

    fresh = 0;
    if (item->field_1 == 0xF2) {
        fresh = (item->field_D >> 7) == 0;
    }
    if (fresh) {
        item->field_D |= 0x80;
        func_800498E4(0x226, func_800AE674(item));
    } else if (func_800ACEB4(item) != 2) {
        hit = 0;
        if (((obj->field_E4 >> 1) & 1) || func_800A692C(obj, 0xC) != 0) {
            hit = 1;
        }
        if (hit) {
            func_800ACD34(item);
            func_800498E4(0x227, func_800AE674(item));
        }
    }
    if (item->field_0 == 0x10) {
        item->field_C |= 1;
    }
    if (func_801F27DC(item) != 0) {
        D_8014767C |= 0x40;
    }
}
