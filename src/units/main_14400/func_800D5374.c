#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { void *value; const VTable *vtable; } Handle;
extern const VTable D_80149DB8, D_80149DA8;
extern void *D_80147FD0[];
/* This base constructor clears the pointer field of an eight-byte handle. */
extern void func_8013687C(void **value);
extern void *func_800D561C(void **arg0, u8 arg1);

/* index is an int: both original callers pass an unmasked int (a0 is the caller's
 * own argument at the 0x800D5418 call; daddu a0,s1 at 0x80122698) and the body
 * narrows it to a byte (andi s0,s0,0xFF at 0x800D53B0). */
s32 func_800D5374(s32 index) {
    Handle handle;
    Handle *self = &handle;
    void *result;
    self->vtable = &D_80149DB8;
    func_8013687C(&self->value);
    self->vtable = &D_80149DA8;
    handle.value = D_80147FD0[(u8)index];
    result = func_800D561C(&self->value, 0);
    self->vtable = &D_80149DA8;
    handle.value = 0;
    self->vtable = &D_80149DB8;
    return result != 0;
}
