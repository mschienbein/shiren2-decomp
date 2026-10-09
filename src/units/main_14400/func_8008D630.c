#include "common.h"

typedef struct Stream Stream;
extern s32 func_8008DF04(void *);
extern u32 func_8008E0C4(Stream *stream, void *dst, u32 len);

/* EHDR chunk handler (table D_8013FF40). The chunk dispatcher func_8008E4B0 calls
 * every handler as (stream, chunk size, context) at 0x8008E554..0x8008E560; this
 * handler reads its own word count, so size and context are unused. */
s32 func_8008D630(Stream *stream, s32 size, void *context) {
    unsigned char buffer[8];
    u32 remaining = func_8008DF04(stream);
    while (remaining) {
        u32 count = remaining;
        if (remaining > 8) {
            count = 8;
            remaining -= 8;
        } else {
            remaining = 0;
        }
        func_8008E0C4(stream, buffer, count);
    }
    return 0;
}
