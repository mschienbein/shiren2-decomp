#include "common.h"
/* RNG object: +0 state buffer, vtable at +0x0C. Slot +0x2C is the state size getter
 * (func_800C5C9C/func_800C5DBC in D_80154058/D_80154088), called with the adjusted receiver. */
typedef struct {
    char fields_0[0x28];
    short adjustment_28;
    s32 (*size_2C)(void *self);
} RngMethods;
typedef struct { void *state_0; char fields_4[8]; RngMethods *methods_C; } Source;
/* Stream read slot +0x28/+0x2C: void (receiver, s32 count, destination pointer). */
typedef struct {
    char fields_0[0x28];
    short adjustment_28;
    void (*read_2C)(void *self, s32 count, void *destination);
} StreamMethods;
typedef struct { char fields_0[0x18]; StreamMethods *methods_18; } Destination;
void func_800C5BE8(Source *source, Destination *destination) {
    s32 size = source->methods_C->size_2C((char *)source + source->methods_C->adjustment_28);
    destination->methods_18->read_2C(
        (char *)destination + destination->methods_18->adjustment_28, size, source->state_0);
}
