#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char state_00; unsigned char pad_01[3]; u32 cursor_04; u32 size_08; signed char *bytes_0C; } Reader;
typedef struct { signed char first, second; } Bytes;
static inline void copy_bytes(Bytes *dst, Bytes *src) {
    *dst = *src;
}
u16 func_8008DFE4(Reader *reader) {
    union { u16 value; Bytes bytes; } result;
    signed char *bytes;
    u32 cursor, end;
    result.value = 0;
    bytes = reader->bytes_0C;
    cursor = reader->cursor_04;
    if (reader->state_00 == 1) {
        end = cursor + 2;
        if (reader->size_08 >= end) {
            copy_bytes(&result.bytes, (Bytes *)(bytes + cursor));
            if (end == reader->size_08) reader->state_00 = 2;
            reader->cursor_04 = end;
        } else reader->state_00 = 2;
    }
    return result.value;
}
