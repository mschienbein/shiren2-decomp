#include "common.h"

typedef unsigned char u8;
typedef struct Obj Obj;
typedef struct VTable VTable;
typedef struct { VTable *field0; Obj *field4; } Request;
typedef struct Shared Shared;
extern Shared D_80140160;
extern void *func_800A65E4(u8 *p, Obj *q, void *target);
extern void func_800A665C(Obj *obj, u8 *value);
extern s32 func_80049CB4(s32 id, ...);
/* The argument is stored as a pointer, then passed to func_800F7A1C by func_800F8360. */
extern Request *func_800F8348(Request *out, Obj *actor);
extern void *func_80093B58(Shared *self, Request *request);
extern s32 func_800D8FF0(void *obj);

s32 func_800F7970(Obj *self, void *other)
{
    Request request;
    u8 first;
    u8 second;
    void *result;
    func_800A65E4(&first, self, other);
    func_800A665C(self, &first);
    func_800A65E4(&second, other, self);
    func_800A665C(other, &second);
    func_80049CB4(2);
    func_800F8348(&request, self);
    result = func_80093B58(&D_80140160, &request);
    if (result == 0) {
        return 0;
    } else {
        return !func_800D8FF0(result);
    }
}
