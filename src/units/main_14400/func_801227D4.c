#include "common.h"

typedef struct {
    unsigned char pad0[0x18];
    short delta;
    short index;
    void (*fn)(void *self, s32 kind, void *data);
} VTable;

typedef struct {
    unsigned char pad0[0x18];
    VTable *vt;
} Obj;

extern const char D_8015F984[]; /* "Memento" */
extern unsigned char D_80148780[];

void func_800CA4A4(void *, void *);

void func_801227D4(void *a)
{
    Obj *obj = a;

    func_800CA4A4(obj, (void *)D_8015F984);
    obj->vt->fn((char *)obj + obj->vt->delta, 9, D_80148780);
}
