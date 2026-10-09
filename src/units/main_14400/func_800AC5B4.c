#include "common.h"

extern s32 D_80143094[];
extern void *func_800AFA7C(void *);
extern void *func_800AFAFC(void *);

/* Callers supply size; the fixed-pool choice depends only on alternate. */
void *func_800AC5B4(s32 size, s32 alternate) {
    void *result;
    if (alternate == 0) {
        result = func_800AFA7C(D_80143094);
    } else {
        result = func_800AFAFC(D_80143094);
    }
    return result;
}
