#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
typedef struct ShirenDirection { s8 value; } ShirenDirection;
typedef struct { unsigned char pad[0x40]; short field40; s32 (*field44)(void *, void *, unsigned char *); } Methods;
typedef struct { unsigned char pad[8]; unsigned char field8; unsigned char pad9[0x1B]; Methods *field24; } Object;
typedef struct { s32 words[2]; } Position;
typedef struct { unsigned char pad[0x1C]; s32 field1C; unsigned char pad20[8]; } Iterator;
extern void *func_800A39C0(Position *, Object *, unsigned char *, s32);
extern void *func_800C5150(Iterator *, Position *, ShirenDirection, s32, u32);
extern void *func_800C51B8(Iterator *);
extern s32 func_800A674C(Object *, void *);
extern s32 func_800A65B8(Object *, void *);
extern u8 func_800A6420(Object *, void *);
extern void func_800A2F80(unsigned char *, s32);
static inline s32 iterator_remaining(Iterator *iterator) {
    return iterator->field1C;
}
void *func_800A4B24(Object *p, s32 expected, s32 ranked, s32 checked) {
    Iterator iterator;
    Position position;
    unsigned char direction;
    ShirenDirection scan_direction;
    unsigned char rank = 0;
    void *best = 0;
    u32 best_rank;
    s32 remaining;
    s32 best_distance;
    best_distance = 0x4C;
    best_rank = 0;
    remaining = 8;
    direction = p->field8;
    for (;;) {
        s32 active = --remaining;
        if (active == -1) break;
        func_800A39C0(&position, p, &direction, 1);
        scan_direction.value = direction;
        func_800C5150(&iterator, &position, scan_direction, 0xFF, 0x4000);
        for (;;) {
            s32 present = iterator_remaining(&iterator);
            void *candidate;
            s32 distance;
            Methods *methods;
            if (!present) break;
            candidate = func_800C51B8(&iterator);
            if (checked) {
                s32 result = func_800A674C(p, candidate) ^ 1;
                if (result) continue;
            }
            methods = p->field24;
            if (methods->field44((unsigned char *)p + methods->field40, candidate, &rank) != expected) break;
            if (ranked && rank < best_rank) break;
            distance = func_800A65B8(p, candidate);
            if (func_800A6420(p, candidate) != 3 && distance < best_distance) {
                best = candidate;
                best_rank = rank;
                best_distance = distance;
            }
        }
        func_800A2F80(&direction, 1);
    }
    return best;
}
