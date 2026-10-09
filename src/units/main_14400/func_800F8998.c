#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    Pos pos;
} Obj;

extern s32 func_80049CB4(s32 id, ...);
extern void func_800A58FC(void *actor, Pos *position);

void func_800F8998(Obj *obj, Pos *pos) {
    obj->pos = *pos;
    func_80049CB4(0x10A3, obj);
    func_80049CB4(0x12D);
    func_800A58FC(obj, pos);
}
