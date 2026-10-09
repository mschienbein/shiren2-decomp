#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x24]; const void *field_24; } Object;
extern const u8 D_8015BBF8[];
extern void *func_800A38FC(s32 size);
extern void *func_800EFC70(void *obj, s32 kind, u8 arg);
void *func_80104C40(u8 arg, Object *self) { if (!self) self = func_800A38FC(0xA0); func_800EFC70(self, 0x45, arg); self->field_24 = D_8015BBF8; return self; }
