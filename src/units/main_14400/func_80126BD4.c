#include "common.h"

typedef struct {
    unsigned char unk00[8];
    const void *unk08;
} Object;
extern const unsigned char D_80153AA0[]; /* Address-only view of the 0x44-byte vtable. */
extern void func_800AC68C(void *);

void func_80126BD4(Object *object, s32 flags) {
    object->unk08 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(object);
    }
}
