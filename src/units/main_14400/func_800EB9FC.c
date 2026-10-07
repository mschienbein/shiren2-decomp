#include "common.h"

typedef short s16;

typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self, s32 a, s32 b, unsigned char c, s32 d);
} VtableEntry;

typedef struct {
    char pad0[0x90];
    VtableEntry entry90;
} Vtable800EB9FC;

/* func_800CEC90 constructs this 0x18-byte list at object offset 0xCC. */
typedef struct {
    void *context;
    void *vtable;
    unsigned char *items;
    unsigned char field_C;
    unsigned char field_D;
    unsigned char field_E;
    void *owner;
    unsigned short label;
} List800EB9FC;

typedef struct {
    char pad0[0x24];
    Vtable800EB9FC *vtable;
    char pad28[0xCC - 0x28];
    List800EB9FC listCC;
} Obj800EB9FC;

extern void *func_800CF058(void *target, unsigned char arg1);

void *func_800EB9FC(Obj800EB9FC *obj) {
    if (obj->vtable->entry90.func((char *)obj + obj->vtable->entry90.delta, 2, 9, 0, 0) != 0) {
        return (void *)0;
    }
    return func_800CF058(&obj->listCC, 9);
}
