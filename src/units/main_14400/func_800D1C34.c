#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct {
    u8 pad0[0x18];
    s16 write_delta;
    s16 write_index;
    void (*write)(void *self, s32 size, void *data);
    u8 pad20[0x28 - 0x20];
    s16 read_delta;
    s16 read_index;
    void (*read)(void *self, s32 size, void *data);
} StreamVTable;

typedef struct {
    u8 pad0[0x18];
    StreamVTable *vtable;
} Stream;

typedef struct {
    u8 first[0x14];
    u8 second[0x14];
    u8 pad28[0xBB - 0x28];
    u8 third[0xC];
} Castle;

extern const char D_8015472C[]; /* "Castle" */

void func_800CA4A4(Stream *stream, const char *tag);

void func_800D1C34(Castle *castle, Stream *stream) {
    func_800CA4A4(stream, D_8015472C);
    stream->vtable->write((u8 *)stream + stream->vtable->write_delta, 0x14, castle->first);
    stream->vtable->write((u8 *)stream + stream->vtable->write_delta, 0x14, castle->second);
    stream->vtable->write((u8 *)stream + stream->vtable->write_delta, 0xC, castle->third);
}
