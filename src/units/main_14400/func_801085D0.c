#include "common.h"

typedef unsigned char u8;

typedef struct Obj80108610 Obj80108610;

extern void *func_800A38FC(s32 size);
extern Obj80108610 *func_80108610(Obj80108610 *self, s32 value);

/* Factory slot of D_8015CC64 (void *(u8 variant, void *mem)): construct the 0xA8-byte
 * kind-0x55 actor in caller memory, or in a freshly allocated block. */
void *func_801085D0(u8 variant, void *mem) {
    Obj80108610 *result;

    if (!mem) {
        result = func_80108610(func_800A38FC(0xA8), variant);
    } else {
        result = func_80108610(mem, variant);
    }
    return result;
}
