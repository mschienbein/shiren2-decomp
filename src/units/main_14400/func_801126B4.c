#include "common.h"
typedef short s16;
typedef struct { s16 delta; s16 index; void *fn; } VtblEntry;
typedef struct { char pad[0x18]; VtblEntry *vtbl; } Obj801126B4;
extern s32 D_8015D644[];
void func_8010CB78(void *, Obj801126B4 *);
void func_800CA4A4(Obj801126B4 *, void *);
void func_801126B4(char *self, Obj801126B4 *obj) {
    VtblEntry *e;
    func_8010CB78(self, obj);
    func_800CA4A4(obj, D_8015D644);
    e = &obj->vtbl[3];
    ((void (*)(void *, s32, void *))e->fn)((char *)obj + e->delta, 8, self + 0x10);
}
