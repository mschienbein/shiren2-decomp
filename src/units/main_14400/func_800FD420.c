#include "common.h"
typedef unsigned char u8;
typedef struct Obj Obj;
extern void *func_800A38FC(s32 size);
extern Obj *func_800FD460(Obj *self, u8 value);
Obj *func_800FD420(u8 value, Obj *memory) {
    if (memory) return func_800FD460(memory, value);
    return func_800FD460(func_800A38FC(0xA4), value);
}
