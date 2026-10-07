#include "common.h"

typedef unsigned char u8;

extern void *D_80143090;
extern void *func_800AFB80(void *key);
extern s32 func_800AFD08(void *entry, void *key);
extern s32 func_800AF920(void *entry, u8 arg1);

s32 func_800AFBFC(void *key) {
    void *entry;

    if (key != 0) {
        if (key == D_80143090) {
            return 1;
        }
        entry = func_800AFB80(key);
        if (entry != 0) {
            return func_800AF920(entry, func_800AFD08(entry, key)) ^ 1;
        }
    }
    return 1;
}
