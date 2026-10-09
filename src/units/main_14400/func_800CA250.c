#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[8];
    s16 delta_08;
    s16 index_0A;
    void (*sync_0C)(void *self);
    u8 pad10[8];
    s16 delta_18;
    s16 index_1A;
    void (*write_1C)(void *self, s32 size, void *data);
} StreamVTable800CA250;

typedef struct {
    s32 position_0;
    s32 field_4;
    s32 field_8;
    u8 padC[0x18 - 0xC];
    StreamVTable800CA250 *vtable;
} Stream800CA250;

extern void func_800CA0A8(Stream800CA250 *object, s32 value);
extern void func_800CA1BC(Stream800CA250 *stream);
extern void func_800CA2C0(Stream800CA250 *stream);

void func_800CA250(Stream800CA250 *stream, s32 value) {
    stream->field_4 = value;
    func_800CA0A8(stream, 0);
    stream->vtable->write_1C((u8 *)stream + stream->vtable->delta_18, 4, &stream->field_4);
    func_800CA1BC(stream);
    func_800CA0A8(stream, value);
    func_800CA2C0(stream);
}
