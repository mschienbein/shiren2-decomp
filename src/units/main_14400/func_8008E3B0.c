#include "common.h"
typedef struct { s32 offset, size; } Entry;
typedef struct { s32 base, dataOffset; u32 count; Entry *entries; } Archive;
extern s32 func_8008DF04(void *stream);
extern s32 func_8008E1E8(void *stream);
extern void *func_80091450(u32 size);
s32 func_8008E3B0(void *stream, Archive *archive) {
    s32 result = 0;
    Archive *out = archive;
    s32 consumed;
    Entry *entries;
    u32 i;

    out->count = func_8008DF04(stream);
    consumed = 4;
    if (func_8008E1E8(stream) != 0) {
        result = -1;
    }
    if (out->count != 0) {
        entries = func_80091450(out->count * 8);
        if (entries != 0) {
            for (i = 0; i < out->count; i++) {
                entries[i].offset = func_8008DF04(stream);
                consumed += 4;
                if (func_8008E1E8(stream) != 0) {
                    result = -1;
                    break;
                }
                entries[i].size = func_8008DF04(stream);
                consumed += 4;
            }
            out->dataOffset = consumed + 4;
            out->entries = entries;
        } else {
            result = -2;
        }
    } else {
        result = -3;
    }
    return result;
}
