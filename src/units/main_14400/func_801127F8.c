#include "common.h"

typedef struct {
    short delta;
    short index;
    void (*fn)(void *self, s32 flags);
} VEntry;

typedef struct {
    unsigned char pad0[0x8];
    VEntry dtor;
} VTable;

typedef struct {
    unsigned char pad0[0x8];
    VTable *vt;
} Obj;

typedef struct {
    s32 type;
    void *actor_4;
    void *target_8;
    unsigned char padC[0x1C - 0xC];
    void *unit_1C;
} Ev;

/* Filled by func_801124F8: the actor pointer is stored unchanged at +0x0. */
typedef struct {
    void *actor_0;
    s32 field_4;
    s32 field_8;
    short value;
    unsigned short flags;
    unsigned char field_10;
} Result;

void func_801124F8(Obj *src, void *actor, void *unit, Result *out);
void func_800A7ADC(void *target, Result *result);
void func_800D3650(void *arg);

void func_801127F8(Obj *obj, Ev *ev)
{
    Result result;

    func_801124F8(obj, ev->actor_4, ev->unit_1C, &result);
    func_800A7ADC(ev->target_8, &result);
    func_800D3650(obj);
    if (obj != 0) {
        obj->vt->dtor.fn((char *)obj + obj->vt->dtor.delta, 3);
    }
}
