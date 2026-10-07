#include "common.h"
typedef unsigned char u8;
extern u8 D_80143094[];
void *func_800AFD78(void *table, u8 index);
void *func_800DAB68(void *item);
void func_800D01B8(void *pair, void *target, void *item);
void func_800DAA58(u8 id, void *sub) {
    void *item = func_800AFD78(D_80143094, id);
    func_800D01B8(sub, func_800DAB68(item), item);
}
