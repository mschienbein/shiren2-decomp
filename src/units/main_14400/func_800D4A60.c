#include "common.h"

typedef unsigned char u8;

typedef struct {
    void *table_0;
} Obj_800D4A60;

extern u8 D_80143094[];
extern s32 func_800AF920(void *table, u8 key);
extern void *func_800AFD78(void *table, u8 key);
extern u8 func_800AFFD0(void *src_table, void *obj, void *dst_table);

void *func_800D4A60(Obj_800D4A60 *obj, s32 wide_key) {
    u8 key = wide_key;
    void *entry;

    if (obj->table_0 == 0) {
        return 0;
    }
    if ((func_800AF920(obj->table_0, key) ^ 1) != 0) {
        return 0;
    }
    entry = func_800AFD78(obj->table_0, key);
    if (entry == 0) {
        return 0;
    }
    return func_800AFD78(D_80143094, (u8)func_800AFFD0(obj->table_0, entry, D_80143094));
}
