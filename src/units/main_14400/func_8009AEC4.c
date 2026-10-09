#include "common.h"

typedef unsigned char u8;
typedef struct { s32 a; s32 b; } Pair800489E0;
typedef struct { s32 kind; s32 x; s32 y; s32 z; } Descriptor;
/* Text window subobject (func_800486A4 view): position, callback target, handle. */
typedef struct { Pair800489E0 pos; void *target; s32 handle; } Window8009AEC4;
/* Callback record filled by func_80095E58: owner back-pointer at +4, pair at +8. Its leading
 * word is the callback vtable pointer (constructors install D_80151EC8 at object +0x60; the
 * draw consumer dispatches through its +0x0C entry); this function only forwards the record. */
typedef struct { const void *vtable_0; void *owner_4; Pair800489E0 pair_8; } Callback8009AEC4;
typedef struct {
    u8 pad_00[0x50];
    Window8009AEC4 window_50;
    Callback8009AEC4 callback_60;
} Object8009AEC4;
extern const Pair800489E0 D_801527F4;
extern Descriptor D_80138D78;
extern void func_80095E58(void *dst, void *object, Pair800489E0 pair);
extern void func_800486A4(void *dst, Descriptor *desc, void *target);
extern void func_80095DA0(void);

void func_8009AEC4(Object8009AEC4 *object)
{
    void *target = &object->callback_60;
    func_80095E58(target, object, D_801527F4);
    func_800486A4(&object->window_50, &D_80138D78, target);
    func_80095DA0();
}
