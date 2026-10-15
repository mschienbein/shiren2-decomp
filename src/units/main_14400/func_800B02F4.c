#include "common.h"
#include "item_records.h"
extern void func_800B0164(void *self, void *stream);
void func_800B02F4(void *stream) {
    s32 i;
    for (i = 1; i < 7; i++) {
        func_800B0164(D_80153AE4[i], stream);
    }
}
