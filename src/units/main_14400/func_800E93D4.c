#include "common.h"

typedef struct { s32 field_00, field_04; } Pair;
/* One-byte direction aggregate, passed by value to func_800A2758. */
typedef struct { unsigned char value; } Dir;
typedef struct { Pair field_00; Dir field_08; } Source;
extern void func_800A2758(Pair *, Dir);
extern s32 func_800AD714(unsigned char *, Pair *);
static inline Pair *copy_pair(Pair *out, const Pair *in) {
    out->field_00 = in->field_00;
    out->field_04 = in->field_04;
    return out;
}
Pair *func_800E93D4(Pair *out, Source *source, unsigned char *state) {
    Pair value;
    Pair *pair = copy_pair(&value, &source->field_00);
    if (*state == 10) func_800A2758(pair, source->field_08);
    {
        s32 failed = func_800AD714(state, pair) ^ 1;
        if (failed) { pair->field_00 = 0; pair->field_04 = 0; }
    }
    return copy_pair(out, pair);
}
