#include "common.h"

typedef struct {
    s32 unk00;
    const void *unk04;
} Object;
extern const unsigned char D_801588C8[]; /* Address-only view of the 0x30-byte vtable. */
extern Object *func_800DA904(Object *, s32, unsigned char *);

Object *func_800DDA64(Object *object, unsigned char *arg1) {
    func_800DA904(object, 0x27, arg1);
    object->unk04 = &D_801588C8;
    return object;
}
