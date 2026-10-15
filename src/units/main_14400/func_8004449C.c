#include "common.h"

/* Write slot (vtable +0x1C) of the read-only stream D_8014A8E8: the stream
 * write contract void (void *self, s32 size, const void *data) with an empty
 * body. ROM 0x174DC is jr ra / nop: none of a0..a2 is read. */
void func_8004449C(void *unused_self, s32 unused_size, const void *unused_data) {
}
