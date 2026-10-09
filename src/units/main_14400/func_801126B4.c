#include "common.h"
typedef short s16;
typedef struct { s16 delta; s16 index; void *fn; } VtblEntry;
typedef struct { char pad[0x18]; VtblEntry *vtbl; } Obj801126B4;
/* "Arrow": NUL-terminated type-name tag in rodata, read byte-wise by func_800CA584. */
extern const char D_8015D644[];
void func_8010CB78(void *, Obj801126B4 *);
void func_800CA4A4(Obj801126B4 *, const char *name);
void func_801126B4(char *self, Obj801126B4 *obj) {
    VtblEntry *e;
    func_8010CB78(self, obj);
    func_800CA4A4(obj, D_8015D644);
    e = &obj->vtbl[3];
    ((void (*)(void *, s32, void *))e->fn)((char *)obj + e->delta, 8, self + 0x10);
}
