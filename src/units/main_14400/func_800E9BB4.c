#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct Obj800E9BB4 Obj800E9BB4;
typedef struct {
    s16 delta;
    s16 index;
    u32 *(*fn)(Obj800E9BB4 *self, u8 level);
} VtblEntry800E9BB4;
typedef struct {
    u8 pad0[0xA0];
    VtblEntry800E9BB4 getThreshold;
} Vtbl800E9BB4;
struct Obj800E9BB4 {
    u8 pad0[0x24];
    Vtbl800E9BB4 *vtbl;
    u8 pad28[0x50];
    u32 value;
};

u32 func_800E9BB4(Obj800E9BB4 *self, u8 level) {
    u32 value = self->value;
    u32 threshold;

    threshold = *self->vtbl->getThreshold.fn((Obj800E9BB4 *)((u8 *)self + self->vtbl->getThreshold.delta), level);
    if (threshold >= value) {
        return threshold;
    }
    if (level == 0x63) {
        return value;
    }
    return *self->vtbl->getThreshold.fn((Obj800E9BB4 *)((u8 *)self + self->vtbl->getThreshold.delta), level + 1) - 1;
}
