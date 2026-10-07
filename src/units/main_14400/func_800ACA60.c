#include "common.h"
typedef unsigned char u8;
extern u8 *D_8015380C[];
u8 func_800AE98C(u8 *);
void func_800B0B10(s32);
s32 func_800B0954(void);
u8 func_800B09A0(void *);
static inline u8 *func_800ACA60_prepare(u8 *slot, u8 *key) {
    slot += func_800AE98C(key);
    if (*slot != 0) {
        func_800B0B10(*slot);
    }
    return slot;
}
s32 func_800ACA60(u8 *key, void *arg) {
    u8 *slot = D_8015380C[*key];
    if (slot == 0 || (slot = func_800ACA60_prepare(slot, key), func_800B0954() == 0)) {
        return 0;
    }
    *slot = func_800B09A0(arg);
    return 1;
}
