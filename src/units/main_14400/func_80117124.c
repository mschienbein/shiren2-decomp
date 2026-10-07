#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct Obj Obj;
typedef struct { s16 delta; s16 index; void (*func)(Obj *, s32, void *); } VEntry;
struct Obj { u8 pad[0x18]; VEntry *vtbl; };
extern void func_800AF11C(void *, Obj *);
extern void func_800CA4A4(Obj *, void *);
extern u8 D_8015DAEC[];
void func_80117124(u8 *a, Obj *o){
    func_800AF11C(a, o);
    func_800CA4A4(o, D_8015DAEC);
    o->vtbl[3].func((Obj *)((u8 *)o + o->vtbl[3].delta), 4, a + 0xC);
}
