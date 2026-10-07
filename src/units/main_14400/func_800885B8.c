#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x4];
    s16 state;
    u8 pad6[0xE];
    s32 id;
    u8 pad18[0xC];
    s32 handle;
} Obj;

extern u8 D_801C3395[];
extern s32 func_8005B07C(void);
extern void func_8005A464(s32 mode, void *params, s32 blend, s32 arg3);
extern void func_800751B4(s32 id, s32 arg);
extern void func_80075E5C(s32 id, s32 arg);
extern void func_80075F44(s32 handle, s32 a, s32 b);
extern void func_80074B90(s32 id);

void func_800885B8(Obj *obj) {
    if (D_801C3395[obj->id] != 0) {
        D_801C3395[obj->id] = 0;
        if (obj->id == func_8005B07C()) {
            func_8005A464(-1, 0, 0, 0);
        }
        func_800751B4(obj->id, 0);
        func_80075E5C(obj->id, 5);
        func_80075F44(obj->handle, 0, 0);
        func_80074B90(obj->id);
    }
    obj->state = 4;
}
