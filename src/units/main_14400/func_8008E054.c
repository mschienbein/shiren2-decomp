#include "common.h"
typedef struct { unsigned char status; unsigned char pad01[3]; u32 position; u32 length; signed char *bytes; } Stream;
typedef struct { signed char high; signed char low; } BytePair;
short func_8008E054(Stream *stream) {
    short result;
    signed char *buffer;
    u32 position;
    u32 next;
    result = 0;
    buffer = stream->bytes;
    position = stream->position;
    if (stream->status == 1) {
        next = position + 2;
        if (stream->length >= next) {
            *(BytePair *)&result = *(BytePair *)(buffer + position);
            if (next == stream->length) stream->status = 2;
            stream->position = next;
        } else stream->status = 2;
    }
    return result;
}
