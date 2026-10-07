#include "common.h"
typedef struct { unsigned char pad_00[0x18]; short offset_18; short field_1A; void (*method_1C)(void *, s32, void *); } Methods;
typedef struct { unsigned char pad_00[0x18]; Methods *field_18; } Object;
extern unsigned char D_8015FE74[];
extern void func_801163B0(void *object, Object *target);
extern void func_800CA4A4(Object *object, void *value);
void func_80124470(unsigned char *object, Object *target) {
    func_801163B0(object, target);
    func_800CA4A4(target, D_8015FE74);
    target->field_18->method_1C((unsigned char *)target + target->field_18->offset_18, 1, object + 0x10);
}
