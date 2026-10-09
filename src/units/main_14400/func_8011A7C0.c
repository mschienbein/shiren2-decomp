#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pos8011A7C0;

typedef struct {
    Pos8011A7C0 pos;
    u8 pad08[0x1E - 0x8];
    u8 flags_1E;
} Obj8011A7C0;

typedef struct Item Item;

extern void *func_800E8A68(Obj8011A7C0 *obj, u8 arg1);
extern char *func_800AE674(void *obj);
extern s32 func_8010BAE8(Item *, s32);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);

static inline void copy_pos(Pos8011A7C0 *to, Pos8011A7C0 *from) {
    to->x = from->x;
    to->y = from->y;
}

/* Item vtable slot 8 (use on target): `self` and the third argument are unused here. */
void func_8011A7C0(void *self, Obj8011A7C0 *target, void *arg2) {
    void *item;
    char *name;

    if ((target->flags_1E >> 2) & 1) {
        item = func_800E8A68(target, 3);
    } else {
        item = 0;
    }
    if (item != 0) {
        name = func_800AE674(item);
        if (func_8010BAE8(item, 1)) {
            func_80049CB4(0x2A, target);
            func_800498E4(0xCF, name, func_800AE674(item));
        } else {
            Pos8011A7C0 pos;

            copy_pos(&pos, &target->pos);
            func_80049CB4(0x11D, &pos);
            func_800498E4(0xD0, name);
        }
    } else {
        func_80049CB4(0x132);
        func_800498E4(0x222);
    }
}
