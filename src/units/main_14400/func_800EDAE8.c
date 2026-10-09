#include "common.h"
typedef struct Object Object;
typedef struct { unsigned char pad_0[0xCC]; unsigned char field_CC[0x40]; } Owner;
extern void func_800CF550(Object *object);
void func_800EDAE8(Owner *p) { func_800CF550((Object *)p->field_CC); }
