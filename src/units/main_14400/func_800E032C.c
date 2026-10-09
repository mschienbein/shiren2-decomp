#include "common.h"
typedef struct { short adjustment; short index; void (*transfer)(void *, s32, void *); } Slot;
typedef struct { unsigned char pad0[0x18]; Slot *vtable18; } Serializer;
typedef struct { unsigned char pad0[8]; unsigned char direction8[4]; unsigned char padC[0x1C]; unsigned char field28[0xA]; unsigned char field32; unsigned char pad33[0x42]; unsigned char field75; } Object;
extern const char D_80158C74[];
extern void func_800A7CAC(void *object, void *serializer);
extern void func_800CA4E8(void *serializer, const void *name);
extern void func_800A665C(void *object, unsigned char *direction);
void func_800E032C(Object *object, Serializer *serializer) {
    Slot *slot;
    func_800A7CAC(object, serializer);
    func_800CA4E8(serializer, D_80158C74);
    slot = &serializer->vtable18[5];
    slot->transfer((unsigned char *)serializer + slot->adjustment, 0x1E, (unsigned char *)object + 0x28);
    object->field75 = object->field32;
    func_800A665C(object, object->direction8);
}
