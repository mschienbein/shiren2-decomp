#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0xC]; u16 *cursor; s32 remaining; } Stream80083A60;
void func_800839C4(Stream80083A60 *stream);
u16 func_80083A60(Stream80083A60 *stream) {
    u16 value;
    if (stream->remaining <= 0) {
        func_800839C4(stream);
    }
    value = *stream->cursor++;
    stream->remaining -= 2;
    return value;
}
