#include "common.h"

typedef unsigned char u8;

extern void *func_800AFA7C(void *);
extern s32 func_800AFBFC(void *key);
extern s32 func_800AFD08(void *table, void *obj);
extern void func_800AFEB4(void *arg0, void *arg1, void *arg2, u8 arg3);

/* Look up obj's key; 0xFF when unavailable, else record and return the slot. */
u8 func_800AFFD0(void *src_table, void *obj, void *dst_table) {
    void *key = func_800AFA7C(dst_table);
    u8 slot;

    if (func_800AFBFC(key)) {
        return 0xFF;
    }
    slot = func_800AFD08(dst_table, key);
    func_800AFEB4(src_table, obj, dst_table, slot);
    return slot;
}
