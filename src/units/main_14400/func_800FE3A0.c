#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0x24];
    void *vtbl;
} Obj800FE3A0;

extern u8 D_8015AAC0[];
extern void *func_800A38FC(s32 size);
extern Obj800FE3A0 *func_800EFC70(Obj800FE3A0 *obj, s32 arg1, u8 arg2);

/* Factory slot of D_8015CC64 (dispatched by func_800A8694): construct in place when
 * storage is supplied, otherwise allocate it. */
void *func_800FE3A0(u8 kind, Obj800FE3A0 *storage) {
    if (storage == 0) {
        storage = func_800A38FC(0xA0);
    }
    func_800EFC70(storage, 0x2E, kind);
    storage->vtbl = D_8015AAC0;
    return storage;
}
