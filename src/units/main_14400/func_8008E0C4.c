#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 state;
    u8 pad1[3];
    u32 pos;
    u32 size;
    u8 *base;
} Stream;

extern void *func_80032D94(void *dst, const void *src, u32 len);

u32 func_8008E0C4(Stream *stream, void *dst, u32 len) {
    u32 pos = stream->pos;
    u8 *base = stream->base;
    u32 count = 0;

    if (stream->state == 1) {
        if (pos + len <= stream->size) {
            func_80032D94(dst, base + pos, len);
            pos += len;
            count = len;
            stream->pos = pos;
            if (pos == stream->size) {
                stream->state = 2;
            }
        } else {
            count = stream->size - pos;
            func_80032D94(dst, base + pos, count);
            pos += count;
            stream->pos = pos;
            stream->state = 2;
        }
    }
    return count;
}
