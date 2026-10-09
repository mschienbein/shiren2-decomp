#include "common.h"

/* Stream table +0x28/+0x2C: D_801541F8 binds void func_800CA668(Buffer *, s32, void *). */
typedef struct { unsigned char pad_00[0x28]; short delta_28; short index_2A; void (*method_2C)(void *, s32, void *); } Methods;
typedef struct { unsigned char pad_00[0x18]; Methods *field_18; } Obj;
extern const char D_8015D71C[];
extern unsigned char D_80148644[];
extern void func_800CA4E8(Obj *obj, void *message);
void func_80112E30(Obj *obj)
{
    func_800CA4E8(obj, (void *)D_8015D71C);
    obj->field_18->method_2C((unsigned char *)obj + obj->field_18->delta_28, 4, D_80148644);
}
