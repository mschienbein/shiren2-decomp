#include "func_801EEA38_140958/types.h"
extern void *func_800A8CB0(s32 id);
extern s32 func_800A8974(void *actor);
extern s32 func_80049CB4(s32 event, ...);

void func_801EEA38_140958(void)
{
    s32 i = 30;
    for (;;) {
        S10Actor *actor;
        if (--i == -1) {
            break;
        }
        actor = func_800A8CB0((u8)i);
        if (!func_800A8974(actor) &&
            !actor->vtable24->call14((u8 *)actor + actor->vtable24->delta10)) {
            func_80049CB4(0x1F, actor);
            if ((actor->flags1E >> 3) & 1) {
                func_80049CB4(0x88, actor);
            }
        }
    }
    func_80049CB4(2);
}
