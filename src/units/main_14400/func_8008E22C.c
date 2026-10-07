#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[8];
    u32 count;
    u8 *data;
} Buffer;

u8 *func_8008E22C(Buffer *buf, u32 index)
{
    if (index < buf->count) {
        return buf->data + index;
    }
    return 0;
}
