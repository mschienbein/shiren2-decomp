#include "common.h"
/* Entity slot +0x5C: s32 (*)(void *self, void *msg). */
typedef struct { unsigned char field_0[0x58]; short field_58; short field_5A; s32 (*field_5C)(void *self, void *msg); } VTable;
typedef struct { unsigned char field_0[0x24]; VTable *field_24; } Object;
/* Five-word stack message: kind 0x11 carries the source object in word 1; the rest is unset here. */
typedef struct { s32 kind; void *source; s32 unused[3]; } Message800A7BE4;
s32 func_800A7BE4(Object *arg, void *source) { Message800A7BE4 message; message.kind=0x11; message.source=source; return arg->field_24->field_5C((char *)arg + arg->field_24->field_58,&message); }
