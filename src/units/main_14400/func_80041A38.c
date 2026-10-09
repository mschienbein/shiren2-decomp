#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 id;
    u8 value;
} Entry80041A38;

/* Opaque list header iterated by func_800CF058; its contents are not touched here. */
typedef struct {
    s32 opaque_00;
} List80041A38;

typedef struct {
    u8 pad0[0xCC];
    List80041A38 list_CC;
} Manager80041A38;

extern Manager80041A38 *D_801476B8;

void *func_800CF058(void *target, u8 arg1);

s32 func_80041A38(u8 id)
{
    Entry80041A38 *entry = func_800CF058(&D_801476B8->list_CC, id);

    if (entry != 0) {
        return entry->value;
    }
    return -1;
}
