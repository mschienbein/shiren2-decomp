#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 field00, bit01; } Object;
extern char *func_800ACB40(void *obj);
extern s32 func_800AD468(u32 bit);
extern char *func_80048480(u16 id);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);
extern char *func_800AC990(void *obj);
extern char *func_80083C90(char *dst, char *src);

char *func_8010DE6C(Object *self, char *dst) {
    char *name = func_800ACB40(self);
    s32 unknown = func_800AD468(self->bit01);
    unknown ^= 1;
    if (unknown && name != 0) {
        func_8005EF08(dst, func_80048480(0x2A6), name);
    } else {
        func_80083C90(dst, func_800AC990(self));
    }
    return dst;
}
