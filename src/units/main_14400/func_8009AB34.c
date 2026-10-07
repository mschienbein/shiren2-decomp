#include "common.h"

typedef struct {
    void *container;
    void *item;
} Pair;

typedef struct {
    short delta;
    short index;
    union {
        void *(*container)(void *self);
        void *(*item)(void *self, u32 index);
    } func;
} VtblEntry;

typedef struct {
    char pad0[0x24];
    VtblEntry *vtbl;
} Manager;

typedef struct {
    char pad0[4];
    VtblEntry *vtbl;
} Target;

/* GNU trailing payload: the caller supplies identifiers after the +0x50 header. */
typedef struct {
    char pad0[0x34];
    s32 index;
    char pad38[0x18];
    s32 ids[0];
} Request;

extern Manager *D_801476B8;

extern void *func_800D0180(void *pair);
extern void func_800D01B8(void *pair, void *target, void *item);

Pair func_8009AB34(Request *request) {
    Pair result;
    Target *target;
    VtblEntry *entry;
    void *value;

    func_800D0180(&result);
    entry = &D_801476B8->vtbl[19];
    target = entry->func.container((char *)D_801476B8 + entry->delta);
    entry = &target->vtbl[7];
    value = entry->func.item((char *)target + entry->delta, (u32)request->ids[request->index]);
    func_800D01B8(&result, target, value);
    return result;
}
