#include "common.h"

/* Offset 0x1C holds the stream's numeric PI device address. */
typedef struct { unsigned char pad0[0x1C]; u32 deviceAddress1C; } Obj;
extern u32 func_80043B48(u32 deviceAddress);

u32 func_80044254(Obj *obj, s32 offset) {
    return func_80043B48(obj->deviceAddress1C + offset);
}
