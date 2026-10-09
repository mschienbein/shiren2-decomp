#include "common.h"
extern unsigned short D_80148E00[2];
extern s32 D_80148E04;
extern void *func_800262C0(const void *source, void *destination, s32 count);
void func_80130E50(const void *shortValue, const void *wordValue) {
    func_800262C0(shortValue, D_80148E00, 2);
    func_800262C0(wordValue, &D_80148E04, 4);
}
