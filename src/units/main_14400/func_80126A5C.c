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
    u8 pad0[0x10];
    u8 state_10;
} Trap;

extern const char D_80160474[]; /* "TrapOW" */

void func_8011640C(Trap *a, Stream *o);
void func_800CA4E8(Stream *stream, const char *tag);

void func_80126A5C(Trap *trap, Stream *stream) {
    u8 bytes[2];

    func_8011640C(trap, stream);
    func_800CA4E8(stream, D_80160474);
    stream->vtable->read((u8 *)stream + stream->vtable->read_delta, 1, &bytes[0]);
    bytes[1] = bytes[0] & 7;
    trap->state_10 = bytes[1];
}
