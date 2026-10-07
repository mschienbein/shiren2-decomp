#include "common.h"
typedef unsigned short u16;
typedef struct { short delta; short index; s32 (*fn)(void *); } VtEntry;
typedef struct { s32 unk0; VtEntry *vt; } Sub;
typedef struct { char pad[0xC]; Sub sub; } Obj;
char *func_80121D80(void *obj, char *buffer);
char *func_80048480(u16 id);
char *func_80083D04(char *dst, char *src);
s32 func_800CD278(Sub *);
char *func_800AC990(void *obj);
char *func_80083C90(char *dst, char *src);
s32 func_8005EF08(char *dst, const char *fmt, ...);
char *func_80121F58(Obj *o, char *out){
    if (func_80121D80(o, out)) {
        Sub *s = &o->sub;
        func_80083D04(out, func_80048480(s->vt[4].fn((char *)s + s->vt[4].delta) ? 0xC6 : 0xC7));
    } else if (func_800CD278(&o->sub)) {
        func_80083C90(out, func_800AC990(o));
    } else {
        char *msg = func_80048480(0x247);
        func_8005EF08(out, msg, func_800AC990(o), 0);
    }
    return out;
}
