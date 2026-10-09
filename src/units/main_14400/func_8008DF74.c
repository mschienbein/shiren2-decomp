#include "common.h"

typedef unsigned char u8;
typedef struct FourBytes { u8 bytes[4]; } FourBytes;
typedef union Word { u32 value; FourBytes bytes; } Word;
typedef struct Stream {
    u8 state;
    u8 pad_01[3];
    u32 position;
    u32 length;
    u8 *buffer;
} Stream;

u32 func_8008DF74(Stream *stream)
{
    Word word;
    u8 *buffer;
    u32 position;
    word.value = 0;
    buffer = stream->buffer;
    position = stream->position;
    if (stream->state == 1) {
        if (position + 4 <= stream->length) {
            word.bytes = *(FourBytes *)(buffer + position);
            position += 4;
            if (position == stream->length)
                stream->state = 2;
            stream->position = position;
        } else {
            stream->state = 2;
        }
    }
    return word.value;
}
