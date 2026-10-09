#include "common.h"

typedef unsigned char u8;
typedef struct Position { s32 x, y; } Position;

extern u32 D_8013968C;
extern s32 D_80139694;
extern s32 D_80139798, D_8013979C, D_801397A0, D_801397A4, D_801397A8;
extern u32 func_80048AC8(void);
extern void func_80084A68(void);
extern void func_80084A78(void);
extern void func_80084A00(void);
extern s32 func_80049750(void);
extern void func_80084004(void);
extern void func_80084904(void);
extern void func_80083FE0(s32);
extern void func_80084A90(s32);
extern void func_80083724(s32);
extern s32 func_80084A84(void);
extern void func_80084FD0(s32);
extern s32 func_80046240(void);
extern void func_8005484C(void);
extern s32 func_80054920(void);
extern void func_8006E6D8(void);
extern void func_80084A20(void);
extern void func_80084B80(void);
extern void func_80087A74(void);
extern s32 func_80048B04(Position *);
extern void func_800493B0(void *);
extern void func_8004A86C(void *);
extern void func_8004C7A4(void *, void *);
extern void func_8004C91C(void *, void *);
extern s32 func_80048DD0(Position *, Position *);
extern void func_8004CCD8(void *, Position *, Position *);
extern void func_8004D090(void *, Position *, Position *, Position *);
extern void func_8004D170(void *, s32);
extern void func_8004D588(void *, s32, s32);
extern void func_8004E440(void *, void *);
extern void func_8004EDD0(void *);
extern void func_8004EEBC(Position *, s32);
extern void func_8004EF70(void *);
extern void func_8004F0D0(void *, Position *);
extern void func_8004F52C(void *, Position *, Position *);
extern void func_8004FA4C(Position *, Position *);
extern void func_8004FA0C(void *, u8 *);
extern void func_8004F960(Position *, u8 *, u8 *);
extern void func_8004FA90(Position *);
extern void func_80050270(Position *, Position *);
extern void func_800504DC(Position *, s32);
extern void func_800507F4(Position *);
extern void func_800506E8(void);
extern void func_8005064C(s32 *);
extern void func_800509EC(void *);
extern void func_80050A64(s32);
extern void func_8004A558(void);
extern void func_8004A66C(s32);

/* local-arithmetic-qualification: the GCC 2.8.1 o32 va-mips.h
 * va_arg operation aligns the compiler-provided argument save area. This
 * arithmetic is ABI traversal, not an integer-carried game-object pointer. */
#define NEXT_ARG(type) (args = (char *)(((u32)args + 3) & ~3U) + 4, *(type *)(args - 4))

static inline void copy_position(Position *dst, Position *src)
{
    dst->x = src->x;
    dst->y = src->y;
}

s32 func_80049CB4(s32 action, ...)
{
    char *args;
    s32 valid = 1;
    s32 force = 0;
    Position position;

    D_8013968C = action & 0xFFF;
    if (D_8013968C != 0x126) {
        s32 available = func_80048AC8();
        available ^= 1;
        if (available) return -2;
    }
    args = (char *)__builtin_next_arg(action);
    if (action & 0xF000) {
        if (action & 0x1000) func_80084A68();
        if (action & 0x2000) func_80084A78();
        if (action & 0x4000) force = 1;
        if (action & 0x8000) func_80084A00();
        D_80139694 = force;
    }
    switch (func_80049750()) {
    case 0:
        valid = 0;
        switch (D_8013968C) {
        case 1:
            D_80139798 = 0;
            func_80084004();
            func_80084904();
            func_80083FE0(1);
            func_80084A90(0);
            func_80083724(0);
            D_801397A0 = 0;
            D_8013979C = 0;
            D_801397A8 = 0;
            D_801397A4 = 0;
            break;
        case 2:
            func_80084FD0(func_80084A84());
            func_80084904();
            D_80139798 = 0;
            break;
        case 3:
            if ((func_80046240() ^ 1) != 0) func_8005484C();
            break;
        case 4:
            if ((func_80046240() ^ 1) != 0) {
                while (func_80054920()) func_8006E6D8();
            }
            break;
        case 6: func_80084A20(); break;
        case 7: func_80084B80(); break;
        case 8: func_80087A74(); break;
        case 9: func_80084A68(); break;
        case 10: func_80084A78(); break;
        case 13: func_80084A90(0); break;
        case 14: func_80084A90(1); break;
        }
        break;
    case 1: {
        Position *actor = NEXT_ARG(Position *);
        copy_position(&position, actor);
        if (force != 1) {
            valid = func_80048B04(&position);
            if (valid != 1) break;
        }
        func_800493B0(actor);
        func_8004A86C(actor);
        break;
    }
    case 5: {
        void *first = NEXT_ARG(void *);
        void *second = NEXT_ARG(void *);
        func_800493B0(first);
        func_800493B0(second);
        func_8004C7A4(first, second);
        break;
    }
    case 2: {
        void *actor = NEXT_ARG(void *);
        void *other = NEXT_ARG(void *);
        func_800493B0(actor);
        func_8004C91C(actor, other);
        break;
    }
    case 3: {
        void *actor = NEXT_ARG(void *);
        Position *first = NEXT_ARG(Position *);
        Position *second = NEXT_ARG(Position *);
        func_800493B0(actor);
        if (force != 1) {
            valid = func_80048DD0(first, second);
            if (valid != 1) break;
        }
        func_8004CCD8(actor, first, second);
        break;
    }
    case 4: {
        void *actor = NEXT_ARG(void *);
        Position *first = NEXT_ARG(Position *);
        Position *second = NEXT_ARG(Position *);
        Position *third = NEXT_ARG(Position *);
        func_800493B0(actor);
        func_8004D090(actor, first, second, third);
        break;
    }
    case 7: {
        Position *actor = NEXT_ARG(Position *);
        s32 value = NEXT_ARG(s32);
        copy_position(&position, actor);
        if (force != 1) {
            valid = func_80048B04(&position);
            if (valid != 1) break;
        }
        func_800493B0(actor);
        func_8004D170(actor, value);
        break;
    }
    case 8: {
        void *actor = NEXT_ARG(void *);
        s32 value = NEXT_ARG(s32);
        s32 flags = NEXT_ARG(s32);
        func_800493B0(actor);
        func_8004D588(actor, value, flags);
        break;
    }
    case 9: {
        void *actor = NEXT_ARG(void *);
        void *other = NEXT_ARG(void *);
        func_800493B0(actor);
        func_8004E440(actor, other);
        break;
    }
    case 10: {
        Position *actor = NEXT_ARG(Position *);
        copy_position(&position, actor);
        if (force != 1) {
            valid = func_80048B04(&position);
            if (valid != 1) break;
        }
        func_8004EDD0(actor);
        break;
    }
    case 11: {
        Position *actor = NEXT_ARG(Position *);
        s32 value = NEXT_ARG(s32);
        copy_position(&position, actor);
        if (force != 1) {
            valid = func_80048B04(&position);
            if (valid != 1) break;
        }
        func_8004EEBC(actor, value);
        break;
    }
    case 12: {
        void *item = NEXT_ARG(void *);
        func_8004EF70(item);
        break;
    }
    case 13: {
        void *item = NEXT_ARG(void *);
        Position *pos = NEXT_ARG(Position *);
        func_8004F0D0(item, pos);
        break;
    }
    case 14: {
        void *item = NEXT_ARG(void *);
        Position *first = NEXT_ARG(Position *);
        Position *second = NEXT_ARG(Position *);
        if (force != 1) {
            valid = func_80048DD0(first, second);
            if (valid != 1) break;
        }
        func_8004F52C(item, first, second);
        break;
    }
    case 18: {
        Position *first = NEXT_ARG(Position *);
        Position *second = NEXT_ARG(Position *);
        func_8004FA4C(first, second);
        break;
    }
    case 16: {
        void *pos = NEXT_ARG(void *);
        u8 *direction = NEXT_ARG(u8 *);
        func_8004FA0C(pos, direction);
        /* The original falls through and consumes three further arguments. */
    }
    case 17: {
        Position *pos = NEXT_ARG(Position *);
        u8 *first = NEXT_ARG(u8 *);
        u8 *second = NEXT_ARG(u8 *);
        func_8004F960(pos, first, second);
        break;
    }
    case 19: {
        Position *pos = NEXT_ARG(Position *);
        if (force != 1) {
            valid = func_80048B04(pos);
            if (valid != 1) break;
        }
        func_8004FA90(pos);
        break;
    }
    case 20: {
        Position *first = NEXT_ARG(Position *);
        Position *second = NEXT_ARG(Position *);
        func_80050270(first, second);
        break;
    }
    case 21: {
        Position *pos = NEXT_ARG(Position *);
        s32 value = NEXT_ARG(s32);
        if (force != 1) {
            valid = func_80048B04(pos);
            if (valid != 1) break;
        }
        func_800504DC(pos, value);
        break;
    }
    case 22: {
        Position *pos = NEXT_ARG(Position *);
        if (force != 1) {
            valid = func_80048B04(pos);
            if (valid != 1) break;
        }
        func_800507F4(pos);
        break;
    }
    case 23: func_800506E8(); break;
    case 24: {
        s32 *pos = NEXT_ARG(s32 *);
        func_8005064C(pos);
        break;
    }
    case 25: {
        Position *pos = NEXT_ARG(Position *);
        if (force != 1) {
            valid = func_80048B04(pos);
            if (valid != 1) break;
        }
        func_800509EC(pos);
        break;
    }
    case 26: {
        s32 value = NEXT_ARG(s32);
        func_80050A64(value);
        break;
    }
    case 28: func_8004A558(); break;
    case 27:
    case 30: {
        s32 value = NEXT_ARG(s32);
        func_8004A66C(value);
        break;
    }
    }
    if (force == 1 || valid == 1) return D_80139798++;
    return -2;
}
