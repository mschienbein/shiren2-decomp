#include "common.h"

typedef unsigned char u8;

typedef struct Obj800FCE80 Obj800FCE80;

extern void *func_800A38FC(s32 size);
extern Obj800FCE80 *func_800FCE80(Obj800FCE80 *self, u8 value);

/* Factory slot of D_8015CC64 (void *(u8 variant, void *mem)): construct the 0xA8-byte
 * kind-0x29 actor in caller memory, or in a freshly allocated block. */
void *func_800FCE40(u8 variant, void *mem) {
    Obj800FCE80 *result;

    if (!mem) {
        result = func_800FCE80(func_800A38FC(0xA8), variant);
    } else {
        result = func_800FCE80(mem, variant);
    }
    return result;
}
