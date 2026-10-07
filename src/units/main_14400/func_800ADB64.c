#include "common.h"
typedef struct { s32 field0, field4; } Pair;
extern s32 func_800ADBC8(void *, void *, Pair *, s32, s32);
static inline Pair *copy_pair(Pair *out, const Pair *source) {
    out->field0 = source->field0;
    out->field4 = source->field4;
    return out;
}
s32 func_800ADB64(void *a, void *b, Pair *source, s32 d) {
    Pair copy;
    return func_800ADBC8(a, b, copy_pair(&copy, source), d, 0);
}
