#include "common.h"

typedef unsigned char u8;

/* Pool handle: pool-record pointer at +0, vtable at +4. */
typedef struct {
    void *pool;
    void *vtable;
} Handle;

extern u8 D_80149DB8[];
extern u8 D_80149DA8[];
/* Three-entry table of pool-record addresses: D_801430D4, D_801430E4, D_801430F4. */
extern void *D_80147FD0[3];
void func_8013687C(void **pool);
void func_800D4A28(void **pool, s32 index, void *arg);

void func_800D52D0(void *arg, s32 index) {
    Handle handle;
    Handle *self;
    Handle *done;

    /* inlined constructor */
    self = &handle;
    self->vtable = D_80149DB8;
    func_8013687C(&self->pool);
    self->vtable = D_80149DA8;
    handle.pool = D_80147FD0[0];
    if (arg != 0 && index >= 0 && index < 4) {
        func_800D4A28(&self->pool, index, arg);
    }
    /* inlined destructor */
    done = &handle;
    done->vtable = D_80149DA8;
    handle.pool = 0;
    done->vtable = D_80149DB8;
}
