#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern const char D_80158C74[5];
typedef struct { s16 delta; s16 index; void (*fn)(void *self, s32 arg1, void *arg2); } VtblEntry800E02D0;
typedef struct { u8 pad0[0x18]; VtblEntry800E02D0 entry; } Vtbl800E02D0;
typedef struct { u8 pad0[0x18]; Vtbl800E02D0 *vtable; } Obj800E02D0;
void func_800A7C54(u8 *arg0, Obj800E02D0 *arg1);
void func_800CA4A4(Obj800E02D0 *obj, const char *arg1);
void func_800E02D0(u8 *arg0, Obj800E02D0 *obj) {
    func_800A7C54(arg0, obj);
    func_800CA4A4(obj, D_80158C74);
    obj->vtable->entry.fn((u8 *)obj + obj->vtable->entry.delta, 0x1E, arg0 + 0x28);
}
