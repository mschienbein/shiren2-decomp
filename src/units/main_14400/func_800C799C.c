#include "common.h"

typedef short s16;

typedef struct {
    s32 type;
    s32 payload[5];
} Message800C799C;

typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self, Message800C799C *msg);
} VtableEntry;

typedef struct {
    char pad0[0x58];
    VtableEntry entry58;
} Vtable800C799C;

typedef struct {
    s32 unk0;
    s32 unk4;
} Base800C799C;

typedef struct {
    char pad0[0x24];
    Vtable800C799C *vtable;
    char pad28[0x84 - 0x28];
    Base800C799C sub;
} Obj800C799C;

typedef struct {
    s32 unk0;
    s32 unk4;
} Iterator800C799C;

extern Obj800C799C *D_801476B8;
extern s32 func_800A8FC8(Iterator800C799C *iter, s32 kind);
extern Obj800C799C *func_800A910C(Iterator800C799C *iter);

void func_800C799C(void) {
    Message800C799C msg;
    Iterator800C799C iter;
    Obj800C799C *obj;
    Base800C799C *base;

    msg.type = 0x18;
    D_801476B8->vtable->entry58.func((char *)D_801476B8 + D_801476B8->vtable->entry58.delta, &msg);
    iter.unk0 = 0;
    while (func_800A8FC8(&iter, 8) != 0) {
        Obj800C799C *target;

        obj = func_800A910C(&iter);
        target = obj;
        base = obj != 0 ? &obj->sub : 0;
        if (base->unk4 != 0) {
            target->vtable->entry58.func((char *)target + target->vtable->entry58.delta, &msg);
        }
    }
}
