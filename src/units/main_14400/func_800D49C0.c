#include "common.h"
/* Room state (D_80143434, passed by func_800D3510): room pointer +0, kind +4, active +8. */
typedef struct { void *room; s32 field4; s32 field8; } Object;
extern s32 func_80049CB4(s32 id, ...);
s32 func_800D49C0(void *arg) {
    Object *object = arg;
    if (object->field8) {
        s32 message = 0x23;
        if (!object->field4) message = 0x22;
        func_80049CB4(0x126, message);
    }
    return object->field8;
}
