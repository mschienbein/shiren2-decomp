#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Pool handle: pool-record pointer at +0, vtable at +4. */
typedef struct {
    void *pool;
    const void *vtable;
} Handle;

/* g++-style vtable entry {delta, index, pfn}; slot 1 (+8) is the deleting destructor. */
typedef struct {
    s16 delta;
    s16 index;
    void (*destroy)(void *self, s32 flags);
} DtorSlot;

typedef struct {
    u8 slot0[8];
    DtorSlot dtor_8;
} ObjVTable;

typedef struct {
    u8 pad0[8];
    const ObjVTable *vtable;
} Obj;

extern u8 D_80149DB8[];
extern u8 D_80149DA8[];
/* Three-entry table of pool-record addresses. */
extern void *D_80147FD0[3];
void func_8013687C(void **pool);
void *func_800D4A60(void **pool, s32 index);
void func_800D4A28(void **pool, s32 index, void *arg);

static inline void construct(Handle *self) {
    self->vtable = D_80149DB8;
    func_8013687C(&self->pool);
    self->vtable = D_80149DA8;
}

static inline void destruct(Handle *self) {
    self->vtable = D_80149DA8;
    self->pool = 0;
    self->vtable = D_80149DB8;
}

void func_800D5160(void) {
    Handle src;
    Handle dst;
    s32 i;
    s32 row;

    construct(&src);
    construct(&dst);
    dst.pool = D_80147FD0[2];
    for (i = 0; ; i++) {
        Obj *obj;

        if (i >= 3) {
            break;
        }
        obj = func_800D4A60(&dst.pool, i);
        if (obj != 0) {
            obj->vtable->dtor_8.destroy((u8 *)obj + obj->vtable->dtor_8.delta, 3);
        }
    }
    for (row = 2; ; row--) {
        void *prev;
        void *next;

        if (row <= 0) {
            break;
        }
        prev = D_80147FD0[row - 1];
        src.pool = prev;
        next = D_80147FD0[row];
        dst.pool = next;
        for (i = 0; ; i++) {
            void *entry;

            if (i >= 3) {
                break;
            }
            entry = func_800D4A60(&src.pool, i);
            func_800D4A28(&dst.pool, i, entry);
        }
    }
    i = 0;
    do {
        func_800D4A28(&src.pool, i, 0);
    } while (++i < 3);
    destruct(&dst);
    destruct(&src);
}
