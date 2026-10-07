#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Obj Obj;
typedef struct { s32 unk0; s32 unk4; } Msg;
typedef struct { s16 delta; s16 index; void (*func)(Obj *, Msg *); } VtblEntry;
typedef struct { u8 pad0[0x80]; VtblEntry entry80; } Vtbl;
struct Obj { u8 pad0[0x4C]; Vtbl *vtbl; };
extern s32 D_801528B8;
s32 func_8009BF80(Obj *obj, s32 arg1);
u8 *func_8006A810(void *dst, s32 value, s32 count);
void func_80045A24(s32 sound);
s32 func_8009BAB0(Obj *obj, s32 event) {
    Msg msg;
    if (event == 0x35) {
        if (func_8009BF80(obj, 3) != 0) {
            func_80045A24(0);
        }
    } else if (event == 0x37) {
        func_8006A810(&msg, 0, 8);
        msg.unk4 = D_801528B8;
        obj->vtbl->entry80.func((Obj *)((u8 *)obj + obj->vtbl->entry80.delta), &msg);
        func_80045A24(0);
    }
    return -1;
}
