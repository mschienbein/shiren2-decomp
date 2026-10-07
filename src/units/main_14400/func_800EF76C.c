#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

/* Auxiliary payload is the +8 member of each 16-byte descriptor. */
typedef struct { void *data; s32 size; void *aux_data; s32 aux_size; } StreamData;
extern const StreamData D_80159250[5];

void *func_800EF76C(u8 index) {
    return D_80159250[index - 0x18].aux_data;
}
