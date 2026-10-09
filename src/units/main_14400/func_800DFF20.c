#include "common.h"

typedef struct { s32 index; } Iter;
typedef struct { unsigned char pad0[0x9A]; unsigned short field_9A; } Object;

extern void *D_801476B8;
extern s32 func_800A8FC8(Iter *it, s32 kind);
extern void *func_800A910C(Iter *it);
extern void func_800F03A8(Object *unit);
extern char *func_800A3B20(void *actor);
extern char *func_80048480(unsigned short id);
extern void func_800498E4(s32 id, ...);

static inline Object *current(Iter *it) { return func_800A910C(it); }
static inline s32 blocked(Object *unit) { return unit->field_9A & 0x100; }

/* Command +0x14 run: apply func_800F03A8 to every kind-0x10 unit flagged 0x40 but not 0x100. */
s32 func_800DFF20(void *self /* receiver: unused; supplied by the command vtable +0x14 call */) {
    Iter iterator;
    char *name;

    iterator.index = 0;
    while (func_800A8FC8(&iterator, 0x10)) {
        Object *unit = current(&iterator);
        s32 flags = unit->field_9A;
        s32 active = 0;
        if (flags & 0x40) active = blocked(unit) == 0;
        if (active) func_800F03A8(unit);
    }
    name = func_800A3B20(D_801476B8);
    func_800498E4(0x6B, name, func_80048480(0x230));
    return 0;
}
