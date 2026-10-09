#include "common.h"
typedef unsigned char u8;
extern u8 D_80143144[40][4];
extern u8 D_80143114[40];
extern s32 D_80143110;
extern s32 func_800B0EB0(u8 *, u8 *);
static inline u8 *record_for(u8 id)
{
    s32 index = id - 1;
    return D_80143144[index];
}
void func_800B0EF4(u8 id)
{
    s32 index;
    u8 *key;
    s32 invalid = ((u8)(id - 1) < 40) ^ 1;
    if (invalid) {
        return;
    }
    key = record_for(id);
    for (index = 0; ; index++) {
        if (index >= 40) {
            return;
        }
        if (func_800B0EB0(key, record_for(D_80143114[index]))) {
            break;
        }
    }
    if (index < 40) {
        if (index >= D_80143110 && D_80143110 < 24) {
            D_80143110++;
        }
        id = D_80143114[index];
        while (index > 0) {
            D_80143114[index] = D_80143114[index - 1];
            index--;
        }
        D_80143114[0] = id;
    }
}
