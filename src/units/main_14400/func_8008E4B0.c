#include "common.h"

typedef struct { s32 tag; u32 size; } Chunk8008E4B0;
/* Chunk handler slot: the registered targets (func_8008DC30, func_8008EE30, func_8008FA90, ...)
 * take (stream, s32 size, context) and return an s32 status. */
typedef struct { s32 tag; s32 (*fn)(void *stream, s32 size, void *ctx); } Handler8008E4B0;
typedef struct { u32 count; Handler8008E4B0 **list; } Table8008E4B0;
u32 func_8008E0C4(void *stream, void *dst, u32 size);

/* Walk `total` bytes of (tag, size) chunks, dispatching each to the handler registered for
 * its tag. Stops on the first handler error; an unknown tag is an error (-1). */
s32 func_8008E4B0(void *stream, Table8008E4B0 *table, u32 total, void *ctx) {
    s32 result;

    /* ODD_C: groups the chunk walk so an empty stream breaks straight out with result 0;
     * the group's loop note also keeps the offset/result initialization after the
     * prologue saves, which shapes scheduling. */
    do {
        u32 offset = 0;

        result = 0;
        if (total == 0) {
            break;
        }
        do {
            Chunk8008E4B0 chunk;
            u32 i;

            func_8008E0C4(stream, &chunk, sizeof(chunk));
            for (i = 0; i < table->count; i++) {
                if (chunk.tag == table->list[i]->tag) {
                    result = table->list[i]->fn(stream, chunk.size, ctx);
                    break;
                }
            }
            if (result != 0) {
                break;
            }
            if (i == table->count) {
                result = -1;
                break;
            }
            offset += sizeof(chunk) + chunk.size;
        } while (offset < total);
    } while (0);
    return result;
}
