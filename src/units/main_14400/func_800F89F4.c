#include "common.h"
typedef struct { unsigned char field_00[0x18]; short adjust; short field_1A; void (*call)(void *); } VTable;
typedef struct { unsigned char field_00[0x24]; VTable *field_24; } Object;
extern void func_800A59A4(Object *obj);
extern s32 func_80049CB4(s32 value, ...);
void func_800F89F4(Object *obj)
{
    func_800A59A4(obj);
    func_80049CB4(6);
    func_80049CB4(0x1087, obj);
    func_80049CB4(7);
    func_80049CB4(0xA4, obj);
    obj->field_24->call((unsigned char *)obj + obj->field_24->adjust);
}
