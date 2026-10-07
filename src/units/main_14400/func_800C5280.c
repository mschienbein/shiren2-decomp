#include "common.h"
typedef struct { s32 f0; s32 f4; } Pair;
void func_800C52C0(void *p, Pair *v, unsigned short c);
static inline void Pair_copy(Pair *dst, Pair *src) {
    dst->f0 = src->f0;
    dst->f4 = src->f4;
}
void *func_800C5280(void *p, Pair *src, unsigned short c) {
    Pair tmp;
    Pair_copy(&tmp, src);
    func_800C52C0(p, &tmp, c);
    return p;
}
