#include "common.h"
/* g++ 2.x member-function descriptor: virtual index or typed direct function. */
typedef struct {
    signed short delta, index;
    union { void (*pfn)(void *); signed short delta2; } u;
} MemberFn;
typedef struct { s32 x, y; void *target; s32 handle; } Binding;
typedef struct { s32 x, y, field_08, field_0C; } Descriptor;
typedef struct { unsigned char pad_00[0x5C]; Binding binding_5C; unsigned char member_6C[0x10]; unsigned char pad_7C[0x80]; Descriptor descriptor_FC; } Object8009D570;
extern const MemberFn D_80152AE0;
void func_80095E58(void *dst, Object8009D570 *object, MemberFn fn);
void func_800486A4(Binding *binding, Descriptor *descriptor, void *target);
void func_8009D570(Object8009D570 *object)
{
    void *member = object->member_6C;
    func_80095E58(member, object, D_80152AE0);
    func_800486A4(&object->binding_5C, &object->descriptor_FC, member);
}
