#include "common.h"
typedef unsigned char u8;
typedef struct { void *table; void *vtable; } Handle;
extern u8 D_80149DB8[];
extern u8 D_80149DA8[];
extern void *D_80147FD0[];
extern void func_8013687C(Handle *);
extern void *func_800D561C(void **, u8);
s32 func_800D5484(u8 index)
{
    Handle handle;
    Handle *self = &handle;
    Handle *done;
    void *result;
    self->vtable = D_80149DB8;
    func_8013687C(self);
    self->vtable = D_80149DA8;
    handle.table = D_80147FD0[index];
    result = func_800D561C(&self->table, 2);
    done = &handle;
    done->vtable = D_80149DA8;
    handle.table = 0;
    done->vtable = D_80149DB8;
    return result != 0;
}
