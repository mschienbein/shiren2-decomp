#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 kind;
} Entry8009A2E8;

/* Callback stored at +0x2EC and invoked as s32 (*)(void *); 1 means accepted. */
s32 func_8009A2E8(void *arg) {
    Entry8009A2E8 *entry = arg;
    return entry->kind == 0x11;
}
