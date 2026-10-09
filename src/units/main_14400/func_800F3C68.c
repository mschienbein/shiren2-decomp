#include "common.h"

typedef struct { unsigned char unknown00[0x7c]; unsigned short field7c; } Object;
s32 func_800F3C68(Object *object) { return (object->field7c >> 1) & 1; }
