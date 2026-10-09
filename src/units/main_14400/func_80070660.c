#include "common.h"

/* 64-byte fixed-point matrix (consumed by the gu* matrix routines). */
typedef struct {
    s32 m[4][4];
} Mtx;

extern s32 D_8013D454;
extern s32 D_8013D450;
extern Mtx *D_801A715C[2];
extern Mtx *D_801A7164;

/* Allocate count matrices from the current frame's pool (capacity 400). */
Mtx *func_80070660(u32 count) {
    Mtx *mtx = D_801A7164;

    if (D_8013D454 == 0) {
        return 0;
    }
    if (mtx - D_801A715C[D_8013D450] + count > 400) {
        return 0;
    }
    D_801A7164 = mtx + count;
    return mtx;
}
