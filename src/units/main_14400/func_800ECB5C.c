#include "common.h"

typedef short s16;
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s16 delta;
    s16 index;
    void (*func)(void *self, s32 size, void *data);
} VtableEntry;

typedef struct {
    char pad0[0x18];
    VtableEntry entry18;
} Vtable800ECB5C;

typedef struct {
    char pad0[0x18];
    Vtable800ECB5C *vtable;
} Obj800ECB5C;

/* func_800CE620/func_800CEC90 construct this 0x18-byte embedded list. */
typedef struct {
    void *context;
    void *vtable;
    u8 *items;
    u8 field_C;
    u8 field_D;
    u8 field_E;
    void *owner;
    u16 label;
} List800ECB5C;

typedef struct {
    char pad0[0x84];
    char unk84[0xCC - 0x84];
    List800ECB5C listCC;
} Self800ECB5C;

extern char D_80158F74[];
extern void func_800E95B8(Self800ECB5C *self, Obj800ECB5C *obj);
extern void func_800CA4A4(Obj800ECB5C *obj, void *arg1);
extern void func_800CE918(List800ECB5C *list, Obj800ECB5C *obj);

void func_800ECB5C(Self800ECB5C *self, Obj800ECB5C *obj) {
    func_800E95B8(self, obj);
    func_800CA4A4(obj, D_80158F74);
    obj->vtable->entry18.func((char *)obj + obj->vtable->entry18.delta, 0x34, self->unk84);
    func_800CE918(&self->listCC, obj);
}
