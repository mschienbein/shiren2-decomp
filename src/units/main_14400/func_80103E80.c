#include "common.h"
typedef unsigned char u8;
extern void *func_800A38FC(s32 size);
extern void *func_80103EC0(void *object, u8 kind);
void *func_80103E80(u8 kind, void *storage) {
    if (storage != 0) {
        return func_80103EC0(storage, kind);
    }
    return func_80103EC0(func_800A38FC(0xA4), kind);
}
