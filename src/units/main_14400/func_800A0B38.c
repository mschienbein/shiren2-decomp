#include "common.h"

typedef unsigned short u16;
typedef struct BitStream BitStream;
extern void func_800A0ACC(BitStream *stream, u32 *out, u32 count);

void func_800A0B38(BitStream *stream, u16 *out, u32 count)
{
    union { u32 word; u16 halves[2]; } value;
    func_800A0ACC(stream, &value.word, count);
    *out = value.halves[1];
}
