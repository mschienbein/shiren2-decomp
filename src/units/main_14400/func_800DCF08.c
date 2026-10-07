#include "common.h"

typedef struct { unsigned char bytes[4]; } Bytes;
typedef struct { char pad0; unsigned char field1; char pad2[6]; char field8[8]; Bytes field10; } Object;
extern void func_800DA9DC(unsigned char *,void *);
s32 func_800DCF08(Object *obj,unsigned char *dest) { *dest++=obj->field1; func_800DA9DC(dest++,obj->field8); *(Bytes *)dest=obj->field10; return 6; }
