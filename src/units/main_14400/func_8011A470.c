#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { s32 x; s32 y; } Pos8011A470;
typedef struct { u8 pad0[0x20]; s16 offset_20; s16 pad22; s32 (*fn_24)(void *self); } VTable8011A470;
typedef struct { s32 field_0; VTable8011A470 *vtable_4; } Sub8011A470;
typedef struct { u8 kind; u8 pad1[0xB]; Sub8011A470 sub_C; } Item8011A470;
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 message_id, ...);
void *func_8011422C(Item8011A470 *item);
s32 func_800CD2BC(void *container, void *element);
s32 func_800CD538(void *container, void *item);
s32 func_8011459C(Item8011A470 *item, Pos8011A470 *pos);

void func_8011A470(void *owner, Pos8011A470 *pos, Item8011A470 *item) {
    s32 id;

    if (item != 0 && item->kind == 9) {
        Pos8011A470 copy;
        Pos8011A470 *p = &copy;
        Sub8011A470 *sub;

        p->x = pos->x;
        p->y = pos->y;
        func_80049CB4(0x11D, p);
        sub = &item->sub_C;
        if (sub->vtable_4->fn_24((u8 *)sub + sub->vtable_4->offset_20) != 0) {
            s32 added = func_800CD2BC(func_8011422C(item), owner);
            s32 ok;

            func_80049CB4(6);
            func_800498E4(0xCE);
            func_80049CB4(7);
            func_80049CB4(0x128, 0x7F);
            ok = func_8011459C(item, pos);
            if (added) {
                func_800CD538(func_8011422C(item), owner);
            }
            if (ok) {
                return;
            }
        }
        id = 0x222;
    } else {
        func_80049CB4(0x132);
        id = 0x223;
    }
    func_800498E4(id);
}
