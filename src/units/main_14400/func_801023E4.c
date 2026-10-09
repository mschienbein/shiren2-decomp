#include "common.h"

typedef unsigned char u8;
typedef struct Object Object;
extern void *func_800A38FC(s32 size);
extern Object *func_80102424(Object *self, u8 kind);

Object *func_801023E4(u8 kind, Object *self) {
    if (self != 0) {
        return func_80102424(self, kind);
    } else {
        return func_80102424(func_800A38FC(0xA0), kind);
    }
}
