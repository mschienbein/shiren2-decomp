#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct ShirenDirection { s8 value; } ShirenDirection;
typedef struct { s32 unk[6]; } Iter;
typedef struct { s32 unk0; s32 unk4; } Item;
void *func_800A6538(void *out_direction, void *obj, void *target);
void func_800C27D0(void *iterator, void *origin, ShirenDirection direction, u8 limit, u8 mode);
s32 func_800C28EC(Iter *);
void *func_800C28FC(void *out, void *iterator);
s32 func_800A251C(void *, Item *);
s32 func_80109370(void *arg0, void *arg1, s32 arg2) {
    Iter it;
    Item item;
    ShirenDirection key;
    u8 next;
    next = arg2 + 1;
    func_800A6538(&key, arg0, arg1);
    func_800C27D0(&it, arg0, key, next, 0);
    while (func_800C28EC(&it)) {
        func_800C28FC(&item, &it);
        if (func_800A251C(arg1, &item)) {
            return 1;
        }
    }
    return 0;
}
