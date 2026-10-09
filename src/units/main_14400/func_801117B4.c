#include "common.h"
typedef struct { short delta; short index; void (*fn)(void *, s32, void *); } VEntry;
typedef struct { char pad[0x18]; VEntry *vtbl; } Target;
/* "Wand": NUL-terminated type-name tag in rodata (0x8015D5A4..0x8015D5A8), read byte-wise
 * until NUL by func_800CA584 via func_800CA4A4. */
extern const char D_8015D5A4[];
void func_800AF11C(void *, Target *);
void func_800CA4A4(Target *, const char *name);
void func_801117B4(char *self, Target *t) {
    VEntry *e;
    func_800AF11C(self, t);
    func_800CA4A4(t, D_8015D5A4);
    e = &t->vtbl[3];
    e->fn((char *)t + e->delta, 2, self + 0xC);
}
