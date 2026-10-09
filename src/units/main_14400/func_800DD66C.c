#include "common.h"
typedef struct { s32 unk0; const void *unk4; } State;
extern const unsigned char D_80158838[48];
extern void *func_800DA904(State *, s32, unsigned char *);
State *func_800DD66C(State *arg0, unsigned char *params) {
    func_800DA904(arg0, 0x24, params);
    arg0->unk4 = D_80158838;
    return arg0;
}
