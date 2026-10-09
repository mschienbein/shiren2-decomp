#include "common.h"

typedef unsigned char u8;
typedef struct { u8 *bytes; u32 bit; } BitStream;
extern void func_800A09C4(void *stream, void *buffer, u32 size);

void func_800A0CB8(BitStream *stream, u8 *buffer, s32 size, s32 count)
{
    func_800A09C4(stream, buffer, size);
    --count;
    --size;
    while (count != -1) {
        u32 source_bit = stream->bit;
        u8 *in = &stream->bytes[source_bit >> 3];
        u32 shift = source_bit & 7;
        s32 byte = count / 8;
        u8 *out = &buffer[size - byte];
        s32 out_shift = count - byte * 8;
        if ((*in >> shift) & 1) {
            *out |= 1 << out_shift;
        }
        --count;
        ++stream->bit;
    }
}
