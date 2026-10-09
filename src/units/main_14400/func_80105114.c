#include "common.h"
typedef struct VTable VTable;
typedef struct { unsigned char field_0[0x24]; VTable *field_24; } Object;
extern VTable D_8015BBF8;
extern void func_800EFD28(Object *, s32);
extern void func_800A3918(Object *);
void func_80105114(Object *self, s32 flags) { self->field_24 = &D_8015BBF8; func_800EFD28(self, 0); if (flags & 1) func_800A3918(self); }
