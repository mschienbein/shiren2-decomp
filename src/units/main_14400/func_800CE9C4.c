#include "common.h"

typedef struct { unsigned char pad_00[0x28]; short delta_28; short index_2A; void (*method_2C)(void *, s32, void *); } Methods;
typedef struct { unsigned char pad_00[0x18]; Methods *field_18; } Target;
typedef Target Obj;
typedef struct { unsigned char pad_00[8]; void *field_08; unsigned char field_0C, field_0D, field_0E; } Data;
extern const char D_80154384[];
extern void func_800CA4E8(Obj *obj, void *message);
void func_800CE9C4(void *data, Target *target)
{
    Data *obj = data;
    func_800CA4E8(target, (void *)D_80154384);
    target->field_18->method_2C((unsigned char *)target + target->field_18->delta_28, 1, &obj->field_0C);
    target->field_18->method_2C((unsigned char *)target + target->field_18->delta_28, 1, &obj->field_0D);
    target->field_18->method_2C((unsigned char *)target + target->field_18->delta_28, 1, &obj->field_0E);
    target->field_18->method_2C((unsigned char *)target + target->field_18->delta_28, obj->field_0C, obj->field_08);
}
