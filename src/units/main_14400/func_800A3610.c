#include "common.h"
typedef struct { s32 f0; s32 f4; } Pair;
typedef struct { s32 row; s32 col; s32 f8; s32 colStart; s32 f10; s32 colEnd; } Iter;
static inline void Pair_copy(Pair *dst, Pair *src) {
    dst->f0 = src->f0;
    dst->f4 = src->f4;
}
Pair *func_800A3610(Pair *out, Iter *it) {
    Pair tmp;
    Pair_copy(&tmp, (Pair *)it);
    it->col++;
    if (it->colEnd < it->col) {
        it->col = it->colStart;
        it->row++;
    }
    Pair_copy(out, &tmp);
    return out;
}
