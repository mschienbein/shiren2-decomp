#include "common.h"
/* Joint unit with one definition of the tag shared by these adjacent functions. */
/* Stream vtable: +0x18/+0x1C writes `size` bytes from `data` (func_800CA610); +0x28/+0x2C reads
 * `size` bytes into `data` (func_800CA668). +0x14 holds the failing tag text pointer. */
typedef struct {
    char pad_00[0x18];
    short adjust_18;
    short pad_1A;
    void (*write_1C)(void *self, s32 size, void *data);
    char pad_20[0x28 - 0x20];
    short adjust_28;
    short pad_2A;
    void (*read_2C)(void *self, s32 size, void *data);
} VTable;
typedef struct { char pad_00[0x14]; const char *error_14; VTable *vt; } Stream;
typedef struct { s32 unk0; Stream *stream; } Obj;
/* Only the NUL-terminated tag is an object; the zero word at 0x80154250
 * remains in the following assembly-owned .rodata span. */
const char D_8015424C[] = "OPT";
void func_800CA0A8(Stream *, s32);
void func_800CA4A4(Stream *, const char *);
void func_800CA4E8(Stream *, const void *);
void func_800CA2C0(Stream *);
void func_800CB408(Obj *o){
    Stream *t;
    func_800CA0A8(o->stream, 8);
    func_800CA4A4(o->stream, D_8015424C);
    t = o->stream;
    t->vt->write_1C((char *)t + t->vt->adjust_18, 4, o);
    func_800CA2C0(o->stream);
}
void func_800CB470(Obj *object) {
    Stream *stream;
    func_800CA0A8(object->stream, 8);
    func_800CA4E8(object->stream, D_8015424C);
    stream = object->stream;
    if (stream->error_14 == 0)
        stream->vt->read_2C((char *)stream + stream->vt->adjust_28, 4, object);
}
