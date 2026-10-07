#include "common.h"

typedef struct Record_8012A7C4 Record_8012A7C4;

/* Bank prefix: the channel-record pointer is at +0x10. */
typedef struct {
    char pad0[0x10];
    Record_8012A7C4 *field_10;
} Obj;

extern Obj *D_801CA70C;
extern Obj *D_801CA708;
extern Record_8012A7C4 *D_801CA6F8;

s32 func_8012C3E0(Obj *obj, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 func_8012A158(s32 arg0)
{
    Obj *obj = D_801CA70C;
    s32 result;

    if (obj != 0) {
        D_801CA70C = 0;
    } else {
        obj = D_801CA708;
        if (obj == 0) {
            D_801CA6F8 = 0;
            return 0;
        }
    }
    if (D_801CA6F8 == 0) {
        D_801CA6F8 = obj->field_10;
    }
    result = func_8012C3E0(obj, arg0, 0x80, 0x80, -1);
    D_801CA6F8 = 0;
    return result;
}
