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
    u8 head[5];
    u8 tail[4];
} Museum;

extern const char D_80153210[]; /* "Museum" */

void func_800CA4E8(Stream *stream, const char *tag);

void func_800A17E0(Museum *museum, Stream *stream) {
    func_800CA4E8(stream, D_80153210);
    stream->vtable->read((u8 *)stream + stream->vtable->read_delta, 5, museum->head);
    stream->vtable->read((u8 *)stream + stream->vtable->read_delta, 4, museum->tail);
}
