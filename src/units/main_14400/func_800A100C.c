#include "common.h"
typedef struct { s32 fields[4]; } Value;
typedef struct { unsigned char pad_00[0x38]; short offset_38; short field_3A; void *(*method_3C)(void *, u32); } Methods;
typedef struct { s32 field_00; Methods *field_04; } Child;
typedef struct { Child *field_00; void *field_04; s32 field_08; } Object;
typedef struct { unsigned char pad_00[0x4C]; s32 *field_4C; unsigned char pad_50[0x14]; s32 field_64; s32 *field_68; unsigned char pad_6C[0x14]; } Temporary;
extern s32 D_80152D68[], D_80151EC8[], D_80151E38[];
extern s32 func_8009A2F8(void *object);
extern s32 func_800A0E1C(Object *object, Value *value, s32 mode, s32 (*callback)(void *));
extern Temporary *func_800953C0(Temporary *object);
extern void func_8009E2C0(Temporary *object, void *value, s32 mode);
extern s32 func_800957C0(Temporary *object, Value *value, s32 a, void *b, s32 c);
static inline void construct(Temporary *object) {
    func_800953C0(object);
    object->field_4C = D_80152D68;
}
static inline void destroy(Temporary *object) {
    object->field_4C = D_80151E38;
}
s32 func_800A100C(Object *object) {
    Value value;
    Temporary temporary;
    s32 result = func_800A0E1C(object, &value, 0x1EA, func_8009A2F8);
    if (result != 1) return result;
    {
        Child *child = object->field_00;
        object->field_04 = child->field_04->method_3C((unsigned char *)child + child->field_04->offset_38, value.fields[0]);
    }
    construct(&temporary);
    temporary.field_64 = -1;
    temporary.field_68 = D_80151EC8;
    func_8009E2C0(&temporary, object->field_04, 0x1EB);
    {
        s32 failed = func_800957C0(&temporary, &value, 1, 0, 0) != 1;
        if (failed) {
            destroy(&temporary);
            return -1;
        } else {
            object->field_08 = value.fields[0];
            destroy(&temporary);
            return 1;
        }
    }
}
