#include "common.h"

typedef struct {
    unsigned char pad0[0xC];
    s32 unkC;
} Obj;

typedef struct {
    s32 x;
    s32 y;
} Pos;

extern void func_800826FC(s32 handle);
extern void func_800835F0(u32 index, u32 arg1, u32 arg2, u32 arg3);
extern void func_8008364C(u32 index);

void func_800488F0(Obj *obj, Pos *pos, s32 count, void *prev) {
    if (obj->unkC >= 0) {
        func_800826FC(obj->unkC);
        if (pos->y >= 0 && pos->x >= 0) {
            func_800835F0(obj->unkC, 1, pos->y * (count + 1) * 13, (pos->x << 4) | 4);
        } else {
            func_8008364C(obj->unkC);
        }
    }
}
