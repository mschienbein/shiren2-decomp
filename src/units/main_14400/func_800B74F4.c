#include "common.h"

typedef unsigned char u8;
typedef struct { s32 cursor; s32 state; } Iter;
typedef struct { u8 pad00[0xA]; u8 kind0A; } Obj800B7574;
extern s32 func_800A8FC8(Iter *it, s32 kind);
extern void *func_800A910C(Iter *it);
extern s32 func_800B7574(Obj800B7574 *obj);

void func_800B74F4(s32 kind) {
    Iter it;
    Iter *cursor = &it;
    cursor->cursor = 0;
    while (func_800A8FC8(cursor, 8)) {
        Obj800B7574 *obj = func_800A910C(cursor);
        if (kind == 0) {
            func_800B7574(obj);
        } else if (kind == obj->kind0A && func_800B7574(obj)) {
            break;
        }
    }
}
