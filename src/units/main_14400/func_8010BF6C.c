#include "common.h"

typedef unsigned char u8;
/* func_800CEC90 and func_800CE658 initialize this embedded collection. */
typedef struct {
    void *unk0;
    void *vtbl4;
    void *storage8;
    u8 capacityC;
    u8 countD;
    void *owner10;
    unsigned short flags14;
} Collection;
typedef struct { char pad[0x1E]; u8 flags; char pad1F[0xAD]; Collection fCC; } Map;
extern u32 D_8013960C;
s32 func_8010C38C(void*);
void *func_800CF058(Collection*, u8);
char *func_800AC990(void*);
void func_800498E4(s32, ...);
s32 func_800AE498(void*);
void func_800AE518(void*, Map*, s32, s32);
s32 func_8010BF6C(u8 *obj, Map *mapArg, s32 check){
    Map *map;
    s32 notSet;
    u8 kind;
    void *first;
    void *second;
    s32 found;
    s32 ok;
    notSet = ((mapArg->flags >> 2) & 1) ^ 1;
    if (notSet) return 1;
    map = mapArg;
    kind = obj[0];
    if (check && (kind == 3 || func_8010C38C(obj))) {
        first = func_800CF058(&map->fCC, 9);
        if (first) {
            func_800498E4(0x30, func_800AC990(first));
            return 0;
        }
    }
    first = func_800CF058(&map->fCC, kind);
    second = func_800CF058(&map->fCC, kind == 3 ? 4 : 3);
    found = 0;
    if (func_8010C38C(obj) || (first && func_8010C38C(first)) || (second && func_8010C38C(second))) found = 1;
    if (found) {
        if (check) {
            if (first) { ok = func_800AE498(first) == 1; if (!ok) return 0; }
            if (second) { ok = func_800AE498(second) == 1; if (!ok) return 0; }
        }
    } else {
        if (first && check) { ok = func_800AE498(first) == 1; if (!ok) return 0; }
        second = 0;
    }
    D_8013960C <<= 1;
    if (first) func_800AE518(first, map, 0, 1);
    if (second) func_800AE518(second, map, 0, 1);
    D_8013960C >>= 1;
    return 1;
}
