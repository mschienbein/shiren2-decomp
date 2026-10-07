#include "common.h"
typedef struct { s32 field_0; unsigned short field_4; unsigned char field_6[0x6E]; } Entry;
typedef struct { s32 field_0; unsigned short field_4, field_6; } Object;
extern Entry D_801BA380[];
void func_800854D4(Object *arg)
{
    s32 i = 0;
    s32 count = 0;
    for (; i < arg->field_6; i++) {
        Entry *entry = &D_801BA380[i];
        if (entry->field_4 != 4 && entry->field_0 != 0) count++;
    }
    if (!count) arg->field_4 = 4;
}
