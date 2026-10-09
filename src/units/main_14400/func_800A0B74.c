#include "common.h"
extern void func_800A0ACC(void *stream, u32 *out, u32 count);
void func_800A0B74(void *stream, unsigned char *out, u32 count) {
    u32 value;
    func_800A0ACC(stream, &value, count);
    *out = (unsigned char)value;
}
