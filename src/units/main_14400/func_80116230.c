#include "common.h"

typedef struct { unsigned char f0; unsigned char kind; unsigned char pad[10]; unsigned char flags; } Mon;
extern unsigned char D_80156F24[];
extern char D_80147620[];
void *func_800B31E8(void *pos, s32 team);
u32 func_8011575C(void *self);
s32 func_800C587C(void *rng, unsigned char limit);
s32 func_80116230(Mon *m, void *other) {
    unsigned char kind = m->kind;
    s32 blocked;
    if (kind == 0xE6) {
        return 1;
    }
    blocked = 0;
    if (func_800B31E8(other, 0x15) || ((m->flags >> 1) & 1)) {
        blocked = 1;
    }
    if (blocked) {
        return 0;
    }
    if (kind == 0xD4) {
        return 1;
    }
    if (func_8011575C(m) == 0) {
        return 0;
    }
    return func_800C587C(D_80147620, D_80156F24[kind]);
}
