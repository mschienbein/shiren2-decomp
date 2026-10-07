#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct Obj Obj;
typedef struct { s16 delta; s16 index; void (*func)(void *self, s16 step); } VtblEntry;
typedef struct { u8 pad0[0x78]; VtblEntry entry78; } Vtbl;
struct Obj { u8 pad0[0x1E]; u8 flags; u8 pad1F[5]; Vtbl *vtbl; };
extern u32 D_8013960C;
s32 func_800A99D0(void);
void func_800498E4(s32 id, ...);
s32 func_80049CB4(s32 id, ...);
s32 func_800E0F40(Obj *obj);
u16 func_800E08B0(Obj *obj);
char *func_800A3B20(Obj *obj);
void func_80136910(char *buf, void *source, u32 arg2, u32 arg3, u32 arg4);
void func_800A7ADC(Obj *obj, char *buf);
/* The +0x4C slot supplies the receiver pointer; this override does not use it. */
void func_801181C0(void *arg0, void *source, Obj *obj) {
    char buf[0x18];
    s32 changed;
    s32 cur;
    char *p;
    u8 next;
    u16 count;
    if (func_800A99D0()) {
        func_800498E4(0x222);
        return;
    }
    changed = 0;
    cur = (u8)func_800E0F40(obj);
    if (cur != 1 || func_800E08B0(obj) != cur) {
        changed = 1;
    }
    func_80049CB4(0x1131);
    if (changed) {
        if (obj->flags & 0xC) {
            func_80049CB4(6);
            func_800498E4(0x89, func_800A3B20(obj));
            func_80049CB4(7);
        }
        func_80049CB4(source != 0 ? 0x22 : 0x21, obj, 0, 0x8000);
    }
    D_8013960C <<= 1;
    next = (u8)func_800E0F40(obj);
    func_80049CB4(6);
    obj->vtbl->entry78.func((Obj *)((u8 *)obj + obj->vtbl->entry78.delta), 1 - next);
    func_80049CB4(7);
    count = func_800E08B0(obj);
    if (count >= 2) {
        p = buf;
        func_80136910(p, source, (u32)(s16)(count - 1), 0x19, 8);
        func_800A7ADC(obj, p);
    }
    D_8013960C >>= 1;
}
