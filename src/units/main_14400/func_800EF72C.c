#include "common.h"

typedef unsigned char u8;

/* Five 16-byte stream descriptors: data/size followed by auxiliary data/size. */
typedef struct { void *data; s32 size; void *aux_data; s32 aux_size; } StreamData;
extern const StreamData D_80159250[5];

void *func_800EF72C(u8 id) {
    return D_80159250[id - 0x18].data;
}
