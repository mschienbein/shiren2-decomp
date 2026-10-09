#include "common.h"

typedef unsigned char u8;

typedef struct Query Query;
typedef struct Unit Unit;

typedef struct {
    short id;
    short pad2;
    void *vtable;
    Query *query;
} Obj;

extern Unit *D_801476B8;

s32 func_800D2FB0(Query *q);
s32 func_800EB820(Unit *unit);
s32 func_800EB8B0(s32 arg0);
void func_800EB8C8(Unit *obj, s32 amount);
void func_800D31EC(Query *q);
void func_800EB744(Unit *unit, s32 delta);
s32 func_80049CB4(s32 id, ...);

/* Virtual slot 2 override; always reports that it handled the request. */
s32 func_800D9AFC(Obj *obj)
{
    s32 amount = func_800D2FB0(obj->query);
    s32 count = func_800EB820(D_801476B8);
    s32 percent = func_800EB8B0(count);

    amount -= amount * percent / 100;
    func_800EB8C8(D_801476B8, count);
    func_800D31EC(obj->query);
    func_800EB744(D_801476B8, -amount);
    func_80049CB4(0x89, D_801476B8);
    func_80049CB4(2);
    return 1;
}
