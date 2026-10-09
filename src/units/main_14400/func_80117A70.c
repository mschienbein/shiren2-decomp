#include "common.h"

/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;

typedef struct {
    unsigned char unk00[8];
    const VTable *unk08;
} Object;
extern const VTable D_80153AA0;
extern void func_800AC68C(void *);

void func_80117A70(Object *object, s32 flags) {
    object->unk08 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(object);
    }
}
