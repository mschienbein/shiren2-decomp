#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x, y;
} Point;

typedef struct {
    u32 high : 7;
    u32 active : 1;
    u32 low : 24;
} Flags;

/* Actor method table stored at actor+0x24. */
typedef struct {
    char pad[0x60];
    short field_60;
    void (*field_64)(void *self); /* entity-family slot +0x64 */
    char pad68[0x28];
    short field_90;
    s32 (*field_94)(void *self, s32 a, s32 b, u8 c, s32 d); /* actor status slot +0x94 */
} VTable;

typedef struct {
    Point position;
    char pad8[0x14];
    unsigned short field_1C;
    Flags field_20;
    VTable *field_24;
    char pad28[0xA];
    unsigned char field_32;
    char pad33[0x3F];
    unsigned char field_72;
    char pad73[2];
    unsigned char field_75;
    char pad76[4];
    unsigned char field_7A;
    unsigned short field_7C;
    char pad7E[0x12];
    u32 field_90;
    char pad94[6];
    unsigned short field_9A;
} Obj;

extern char *func_800A3B20(Obj *);
extern s32 func_800E4454(Obj *);
extern s32 func_800E115C(Obj *, s32, s32, unsigned char, s32);
extern s32 func_80049CB4(s32, ...);
extern s32 func_800E0F40(Obj *);
extern void func_800E03BC(Obj *, s32);
extern void func_800E0508(Obj *, s32, s32);
extern void func_800F05EC(Obj *);
extern void func_800498E4(s32, ...);
extern s32 func_800A08D8(s32, s32, s32);

static inline void copyFlags(Flags *out, const Flags *in) {
    *out = *in;
}

/* Actor status slot +0x94 override. */
s32 func_800F212C(Obj *a, s32 b, s32 c, unsigned char d, s32 e) {
    Point position;

    if (c == 2) {
        if (b == 0) {
            Flags flags;

            copyFlags(&flags, &a->field_20);
            {
                u32 active = flags.active;

                if (active) {
                    a->field_1C |= 0x20;
                }
            }
            a->field_90 &= 0xFCFF5FFF;
            a->field_7C &= 0xFEFF;
            func_800E03BC(a, 1);
            func_800E0508(a, 1, 1);
            a->field_9A &= 0xFFF7;
            a->field_72 &= 0xFD;
            a->field_1C &= 0xFDFF;
            a->field_24->field_64((char *)a + a->field_24->field_60);
        }
    } else if (c == 8) {
        if (b != 2) {
            char *old = func_800A3B20(a);

            if (b == 0 && (u8)func_800E0F40(a) == 1) {
                return 0;
            }
            if (b == 1) {
                if (a->field_7A < (u8)func_800E0F40(a)) {
                    a->field_32 = a->field_7A;
                }
            }
            func_800E115C(a, b, c, d, e);
            {
                Point *p = &position;
                s32 event;

                p->x = a->position.x;
                p->y = a->position.y;
                event = func_80049CB4(0x109, p);
                {
                    s32 failed = func_800E4454(a) != 1;

                    if (failed) {
                        a->field_75 = func_800E0F40(a);
                    }
                }
                func_800F05EC(a);
                {
                    s32 failed = func_800E4454(a) != 1;

                    if (failed) {
                        s32 message = 0x197;

                        if (b == 0) {
                            message = 0x28;
                        }
                        func_800498E4(message, old, func_800A3B20(a));
                    }
                }
                func_800A08D8(1, event, 0);
                return 1;
            }
        }
    } else if (c == 9) {
        if (b == 0) {
            if ((a->field_7C >> 9) & 1) {
                return 0;
            }
            a->field_24->field_94((char *)a + a->field_24->field_90, 0, 2, 0xFE, 0);
        }
    }
    return func_800E115C(a, b, c, d, e);
}
