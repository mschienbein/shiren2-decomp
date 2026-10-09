#include "common.h"

typedef unsigned char u8;
typedef struct Obj80100B60 Obj80100B60;

extern void *func_800A38FC(s32 size);
extern Obj80100B60 *func_80100B60(Obj80100B60 *self, u8 kind);

/* Factory: construct into the supplied storage, or allocate 0xA0 bytes. */
Obj80100B60 *func_80100B20(u8 kind, Obj80100B60 *storage) {
    if (storage != 0) {
        return func_80100B60(storage, kind);
    }
    return func_80100B60(func_800A38FC(0xA0), kind);
}
