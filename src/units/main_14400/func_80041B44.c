#include "common.h"

typedef unsigned short u16;
/* Slot +0x68/+0x6C of the player vtable D_80159008 (installed at 0x800EB140) holds
 * func_800E96D4, whose definition returns u32; the wrapper narrows explicitly. */
typedef struct { unsigned char pad_00[0x68]; short delta_68; short index_6A; u32 (*method_6C)(void *); } Methods;
typedef struct { unsigned char pad_00[0x24]; Methods *field_24; } Obj;
extern Obj *D_801476B8;
s32 func_80041B44(void)
{
    Obj *obj = D_801476B8;
    return (u16)obj->field_24->method_6C((unsigned char *)obj + obj->field_24->delta_68);
}
