#include "common.h"

typedef struct { s32 x, y; } Pair;
/* Event built by func_800C4BC0 (vtable D_80153FC8): +0x10 is its target pointer. */
typedef struct {
    unsigned char unk00[0x10];
    void *target_10;
} Object;
extern s32 func_80049CB4(s32 command, ...);

/* Slot +0x0C of D_80153FC8 (0x80153FD4). The base dispatcher func_800C4864 passes the
 * adjusted receiver and two local coordinate pointers (0x800C4934..0x800C4950,
 * 0x800C4A60..0x800C4A78) and consumes the result (s6, forwarded at 0x800C4B40). */
s32 func_800C4C08(Object *object, Pair *from, Pair *to) {
    return func_80049CB4(0xC0, object->target_10, from, to);
}
