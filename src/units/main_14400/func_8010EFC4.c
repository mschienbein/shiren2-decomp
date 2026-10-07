#include "common.h"

typedef short s16;

typedef struct {
    s16 delta;
    s16 index;
    void (*func)(void *stream, s32 count, void *destination);
} VtableEntry;

typedef struct {
    char pad0[0x28];
    VtableEntry entry28;
} Vtable8010EFC4;

typedef struct {
    char pad0[0x18];
    Vtable8010EFC4 *vtable;
} Obj8010EFC4;

typedef struct {
    char pad0[0x20];
    char unk20[2];
} Self8010EFC4;

extern char D_8015D3E4[];
extern void func_8010C420(Self8010EFC4 *self, Obj8010EFC4 *obj);
extern void func_800CA4E8(Obj8010EFC4 *obj, void *arg1);

void func_8010EFC4(Self8010EFC4 *self, Obj8010EFC4 *obj) {
    func_8010C420(self, obj);
    func_800CA4E8(obj, D_8015D3E4);
    obj->vtable->entry28.func((char *)obj + obj->vtable->entry28.delta, 2, self->unk20);
}
