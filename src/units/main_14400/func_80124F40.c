#include "common.h"

typedef struct {
    unsigned char pad_00[0x90];
    signed short delta_90;
    unsigned short reserved_92;
    s32 (*action_94)(void *, s32, s32, unsigned char, s32);
} ActionVtable;
typedef struct {
    unsigned char pad_00[0x1E];
    unsigned char flags_1E;
    unsigned char pad_1F[5];
    ActionVtable *vtable_24;
} Target;
extern s32 func_80049CB4(s32 id, ...);

/* 80116028..80116050 supplies all seven pointers; self, owner, origin, direction and source are unused. */
s32 func_80124F40(void *self, void *owner, void *origin, void *position,
                 void *direction, Target *target, Target *source)
{
    func_80049CB4(0xEA, position);
    if (target != 0 && (target->flags_1E & 0x7C)) {
        ActionVtable *vtable;
        func_80049CB4(0x21, target, 0, 0);
        vtable = target->vtable_24;
        vtable->action_94((char *)target + vtable->delta_90, 0, 0x10, 0xFE, 0);
    }
    return 1;
}
