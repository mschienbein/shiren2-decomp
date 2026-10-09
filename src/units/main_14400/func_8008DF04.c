#include "common.h"

typedef unsigned char u8;
typedef struct { s32 value; } __attribute__((packed)) PackedWord;
typedef struct { u8 state; u8 pad01[3]; u32 offset; u32 length; u8 *data; } Stream;

s32 func_8008DF04(Stream *stream)
{
    PackedWord result;
    u8 *data;
    u32 offset;
    u32 next;
    result.value = 0;
    data = stream->data;
    offset = stream->offset;
    if (stream->state == 1) {
        next = offset + 4;
        if (stream->length >= next) {
            result = *(PackedWord *)(data + offset);
            if (next == stream->length)
                stream->state = 2;
            stream->offset = next;
        } else {
            stream->state = 2;
        }
    }
    return result.value;
}
