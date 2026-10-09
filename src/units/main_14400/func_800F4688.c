#include "common.h"

typedef struct {
    char pad00[0x20];
    u32 flags20;
    char pad24[0x78 - 0x24];
    u32 flags78;
} Obj800F4688;

extern s32 func_800E1CD4(Obj800F4688 *obj, s32 kind);
extern u32 D_80148270;

/* Virtual: refresh active flags from the base set, masking while state 15 holds. */
void func_800F4688(Obj800F4688 *obj) {
    obj->flags20 = obj->flags78;
    if (func_800E1CD4(obj, 0xF)) {
        obj->flags20 &= ~D_80148270;
    }
}
