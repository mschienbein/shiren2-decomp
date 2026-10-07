#include "common.h"

/* Partial write view; the historical owner and API are unresolved. */
typedef struct {
    unsigned char byte_00;
    unsigned char unobserved_01[0x0B];
    u32 word_0C;
} WriteView_8008DD10;

void func_8008DD10(WriteView_8008DD10 *state) {
    state->byte_00 = 0;
    state->word_0C = 0;
}
