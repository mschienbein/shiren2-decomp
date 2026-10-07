#include "common.h"
/* func_8009F680 initializes 0xA2 status bytes at offsets 0x58..0xF9. */
typedef struct { unsigned char field_0[0x54]; s32 field_54; unsigned char field_58[0xA2]; } Object;
extern s32 func_8009FD08(Object *, s32);
s32 func_8009F8A8(Object *arg, s32 index) { if (arg->field_54 && !arg->field_58[index]) return 0x72000000; if (func_8009FD08(arg, index)) return 0x74000000; return 0x78000000; }
