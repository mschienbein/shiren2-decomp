#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { s16 delta, index; void (*read)(void *, s32, void *); } ReadEntry;
typedef struct { u8 pad00[0x28]; ReadEntry read28; } Vtable;
typedef struct { u8 pad00[0x18]; Vtable *vtable; } Stream;
extern const char D_80153694[];
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

extern void func_800CA4E8(Stream *, const void *);
extern void func_800A9C5C(s32);
extern void func_800A9C8C(u8, u8);
void func_800A9608(Stream *stream)
{
    func_800CA4E8(stream, D_80153694);
    stream->vtable->read28.read((u8 *)stream + stream->vtable->read28.delta, 11, &D_80142F24);
    if (D_80142F24.index == 20) {
        func_800A9C5C(0);
    } else {
        func_800A9C8C(D_80142F24.index, D_80142F24.count);
    }
    stream->vtable->read28.read((u8 *)stream + stream->vtable->read28.delta, 1, &D_80142F18.mode);
    stream->vtable->read28.read((u8 *)stream + stream->vtable->read28.delta, 1, &D_80142F18.coordinates[0]);
    stream->vtable->read28.read((u8 *)stream + stream->vtable->read28.delta, 1, &D_80142F18.coordinates[1]);
    stream->vtable->read28.read((u8 *)stream + stream->vtable->read28.delta, 1, &D_80142F18.flags);
    stream->vtable->read28.read((u8 *)stream + stream->vtable->read28.delta, 1, &D_80142F18.status);
}
