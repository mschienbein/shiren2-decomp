#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Five 16-byte rodata stream descriptors (same model as func_800EF72C/func_800EF74C):
 * the buffer whose first word is cleared, its byte size, then the auxiliary buffer and
 * its size. */
typedef struct {
    s32 *buffer_00;
    s32 size_04;
    void *aux_buffer_08;
    s32 aux_size_0C;
} Entry800EF700;

extern const Entry800EF700 D_80159250[5];

void func_800EF700(void)
{
    s32 i;

    for (i = 4; i != -1; i--) {
        *D_80159250[i].buffer_00 = 0;
    }
}
