#include "common.h"
/* Actor slot +0x94: targets func_800EE8E8/func_800EA38C/func_800E115C/... all take
 * (self, s32, s32, unsigned char, s32) and return s32. */
typedef struct { unsigned char field_00[0x90]; short adjust; short field_92; s32 (*call)(void *, s32, s32, unsigned char, s32); } VTable;
typedef struct { unsigned char field_00[0x24]; VTable *field_24; } Object;
extern s32 func_800E2074(Object *obj);
extern s32 func_800E1CC4(Object *obj, s32 value);
extern s32 func_800E4454(Object *obj);
s32 func_800EE504(Object *obj)
{
    s32 result = 0;
    if (func_800E2074(obj) && !func_800E1CC4(obj, 2) && !func_800E4454(obj))
        result = !obj->field_24->call((unsigned char *)obj + obj->field_24->adjust, 2, 9, 0, 0);
    return result;
}
