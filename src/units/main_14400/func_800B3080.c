#include "common.h"

typedef struct { s32 a, b, c, d; } Q;
extern unsigned char D_80143392;
u32 func_800B1C6C(void *pos);
void *func_800B1F90(void *pos);
void *func_800B3024(void *out, void *pos);
void *func_800B3080(Q *out, void *pos) {
    s32 ok = 0;
    if (D_80143392 == 0) { s32 f = func_800B1C6C(pos) & 0x1000; ok = f != 0; }
    if (ok) {
        Q *s = func_800B1F90(pos);
        out->a = s->a; out->b = s->b; out->c = s->c; out->d = s->d;
    } else {
        func_800B3024(out, pos);
    }
    return out;
}
