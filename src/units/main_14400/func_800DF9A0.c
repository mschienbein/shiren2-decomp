#include "common.h"

typedef struct {
    unsigned short unk00;
    const void *unk04;
} Object;
extern const unsigned char D_80157FA8[]; /* 0x30-byte command vtable. */
extern const unsigned char D_80158B08[]; /* 0x30-byte command vtable. */

Object *func_800DF9A0(Object *object) {
    object->unk04 = &D_80157FA8;
    object->unk00 = 0x2F;
    object->unk04 = &D_80158B08;
    return object;
}
