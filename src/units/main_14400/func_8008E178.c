#include "common.h"

typedef unsigned char u8;
typedef float f32;
typedef struct {
    u8 b[4];
} Bytes4;
typedef struct {
    u8 state;
    u32 pos;
    u32 size;
    u8 *data;
} Reader;

f32 func_8008E178(Reader *r)
{
    f32 value = 0.0f;
    u8 *data = r->data;
    u32 pos = r->pos;
    u32 next;

    if (r->state == 1) {
        next = pos + 4;
        if (next <= r->size) {
            *(Bytes4 *)&value = *(Bytes4 *)(data + pos);
            if (next == r->size) {
                r->state = 2;
            }
            r->pos = next;
        } else {
            r->state = 2;
        }
    }
    return value;
}
