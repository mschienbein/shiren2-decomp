#include "common.h"

typedef struct { s32 x; s32 y; } Position;

void func_800A592C(void *actor, Position *position);
s32 func_80049CB4(s32 message, ...);

void func_800A58FC(void *actor, Position *position) {
    func_800A592C(actor, position);
    func_80049CB4(0x88, actor);
}
