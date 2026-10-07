#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

/* Five 16-byte stream descriptors rooted at the first data pointer. */
typedef struct { void *data; s32 size; void *aux_data; s32 aux_size; } StreamData;
extern const StreamData D_80159250[5];
s32 func_800EF74C(u8 id) {
    return D_80159250[id - 0x18].size;
}
