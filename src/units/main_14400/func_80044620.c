#include "common.h"

typedef struct { unsigned char storage[0x50]; } Object;

void *func_80044308(void *obj, s32 a, s32 *status);
void *func_800439A8(void *obj, s32 kind, s32 a, s32 *status);

/* Constructs a kind-specific object in this function's static 0x50-byte storage
 * (.bss 0x80160AA0) and returns it. */
void *func_80044620(s32 kind, s32 value, s32 *status)
{
    static Object D_80160AA0;

    if (kind == 2) {
        func_80044308(&D_80160AA0, value, status);
    } else {
        func_800439A8(&D_80160AA0, kind, value, status);
    }
    return &D_80160AA0;
}
