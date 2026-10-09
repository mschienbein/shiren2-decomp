#include "common.h"
typedef struct { s32 fields00[7]; unsigned char *field1C; s32 field20; } Buffer;
extern void *func_80032D94(void *destination, const void *source, u32 length);
void func_800CA668(Buffer *buffer, s32 length, void *destination) {
    func_80032D94(destination, buffer->field1C + buffer->field20, (u32)length);
    buffer->field20 += length;
}
