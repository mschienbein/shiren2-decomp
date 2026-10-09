#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

/* Run-length decoder: a negative code copies -code literal bytes, a positive code
 * repeats the next byte code + 1 times, zero terminates. Returns the decoded size,
 * or 0 if the output would exceed capacity. */
u32 func_8005EFF0(u8 *dst, u8 *src, u32 capacity)
{
    u32 size = 0;
    u32 count;
    u32 i;

    i = *src;
    i <<= 24;
    if (i != 0) {
        src++;
        do {
            s32 code = (s32)i >> 24;
            if (code < 0) {
                count = -code;
                if (capacity < size + count) {
                    return 0;
                }
                for (i = 0; i < count; i++) {
                    *dst++ = *src++;
                    size++;
                }
            } else {
                count = code + 1;
                if (capacity < size + count) {
                    return 0;
                }
                if (count != 0) {
                    u8 *value = src;
                    i = 0;
                    do {
                        *dst++ = *value;
                        size++;
                    } while (++i < count);
                }
                src++;
            }
            i = *src++;
            i <<= 24;
        } while (i != 0);
    }
    return size;
}
