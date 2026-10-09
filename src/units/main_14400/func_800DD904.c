#include "common.h"
typedef struct { s32 field_00; const void *field_04; } Object;
extern Object *func_800DA904(Object *obj, s32 type, unsigned char *value);
extern const unsigned char D_80158898[48];
Object *func_800DD904(Object *obj, unsigned char *value)
{
    func_800DA904(obj, 0x26, value);
    obj->field_04 = D_80158898;
    return obj;
}
