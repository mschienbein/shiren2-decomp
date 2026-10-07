#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { u8 pad[0xC]; u8 xC; } A;
extern s32 func_800ACEB4(A *);
extern char *func_800ACC90(void *obj);
extern char *func_800ACB40(void *obj);
extern char *func_800ACCDC(void *obj);
extern char *func_80048480(u16 id);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);
extern char *func_80083C90(char *dst, char *src);
extern char *func_80083D04(char *dst, char *src);
void *func_80113564(A *a, void *out){
    s32 r = func_800ACEB4(a);
    if (r == 2) {
        func_80083C90(out, func_800ACC90(a));
    } else if (r == 1) {
        func_80083C90(out, func_800ACC90(a));
    } else {
        char *t = func_800ACB40(a);
        if (t) {
            func_8005EF08(out, func_80048480(0x2A3), t);
        } else {
            func_80083C90(out, func_800ACCDC(a));
        }
    }
    if (a->xC) {
        func_80083D04(out, func_80048480(0xFC));
    }
    return out;
}
