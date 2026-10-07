#include "common.h"
typedef unsigned char u8;
/* Byte-stream reader: func_8008E178/func_8008DF04 read `data + pos` while pos + n <= size. */
typedef struct {
    u8 state;
    u32 pos;
    u32 size;
    u8 *data;
} Reader;
void func_8008DEC0(Reader *);
void func_8008DE64(Reader *reader, u8 *data, u32 size) {
    if (reader->state) {
        func_8008DEC0(reader);
    }
    reader->state = 1;
    reader->pos = 0;
    reader->data = data;
    reader->size = size;
}
