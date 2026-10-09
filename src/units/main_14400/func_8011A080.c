#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { s32 cursor; } Iter;
typedef struct { u8 pad00[0x72]; u8 flags72; } Obj;
typedef struct { u8 kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern u8 D_80143391;
extern u32 D_8013960C;
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern void func_800B23B0(void);
extern s32 func_800A8FC8(s32 *index, s32 kind);
extern void *func_800A910C(Iter *it);
extern void func_800E20F0(Obj *obj);

static inline s32 same_mode(u8 actual, u8 expected) {
    return actual == expected;
}

void func_8011A080(void *self /* unused */, Position *position, void *target /* unused scroll callback input */) {
    Position copy;
    Position *point;
    Iter it;
    Iter *cursor;
    point = &copy;
    point->x = position->x;
    point->y = position->y;
    if (same_mode(D_80142F18.mode & 0xE0, 0x20)) {
        if (!(D_80143391 & 4)) {
            func_80049CB4(0xFB, point);
            func_800498E4(0xCA);
            func_800B23B0();
            func_80049CB4(0xDD);
            D_8013960C <<= 1;
            cursor = &it;
            cursor->cursor = 0;
            while (func_800A8FC8(&cursor->cursor, 0x7C)) {
                Obj *obj = func_800A910C(cursor);
                if (obj->flags72 & 1) func_800E20F0(obj);
            }
            D_8013960C >>= 1;
        } else {
            func_80049CB4(0x132);
            func_800498E4(0x223);
        }
    } else {
        func_80049CB4(0x11D);
        func_800498E4(0x225);
    }
}
