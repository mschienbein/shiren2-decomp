#include "common.h"
typedef struct { unsigned char field_00[0x1C]; unsigned char *field_1C; s32 field_20; } Buffer;
extern void *func_80032D94(void *dest, const void *src, u32 size);
void func_800CA610(Buffer *buffer, s32 size, void *src)
{
    func_80032D94(buffer->field_1C + buffer->field_20, src, (u32)size);
    buffer->field_20 += size;
}
