#include "common.h"

/* Partial signed word views; the historical declarations are unresolved. */
extern s32 D_8013DFEC;
extern s32 D_8013DFF0;
extern s32 D_8013DFF4;
extern s32 D_8013DFF8;

void func_8007F090(s32 arg0, s32 arg1) {
    if (D_8013DFEC == -1 || arg0 < D_8013DFEC) {
        D_8013DFEC = arg0;
    }
    if (D_8013DFF0 == -1 || arg1 < D_8013DFF0) {
        D_8013DFF0 = arg1;
    }
    if (D_8013DFF4 == -1 || D_8013DFF4 < arg0) {
        D_8013DFF4 = arg0;
    }
    if (D_8013DFF8 == -1 || D_8013DFF8 < arg1) {
        D_8013DFF8 = arg1;
    }
}
