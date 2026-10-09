#include "common.h"
typedef unsigned char u8;
typedef struct { void *field_00; void *field_04; } Entry800D01B8;
typedef struct { s32 kind_00; void *vtable_04; u8 pad_08[8]; Entry800D01B8 set_10; } Object800DB410;
extern u8 D_80158478[];
void *func_800DA904(void *object, s32 kind, u8 *params);
Entry800D01B8 *func_800D0180(Entry800D01B8 *set);
void func_800DAA58(u8 id, void *set);
Object800DB410 *func_800DB410(Object800DB410 *object, u8 *params)
{
    func_800DA904(object, 0x10, params++);
    object->vtable_04 = D_80158478;
    func_800D0180(&object->set_10);
    func_800DAA58(*params, &object->set_10);
    return object;
}
