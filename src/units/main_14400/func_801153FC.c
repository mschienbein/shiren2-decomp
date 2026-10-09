#include "common.h"
typedef struct FieldView800D0054 {
    unsigned char unobserved_00[8];
    unsigned char *pointer_08;
    unsigned char unobserved_0C[4];
    unsigned char bytes_10[8];
    void *owner_18;
} FieldView800D0054;
typedef struct { unsigned char pad0[0xC]; FieldView800D0054 field_C; } Obj800AE648;
extern unsigned char *func_800D0054(FieldView800D0054 *object, void *owner);
void func_801153FC(Obj800AE648 *obj) { func_800D0054(&obj->field_C, obj); }
