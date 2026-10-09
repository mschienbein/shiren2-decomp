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
    u8 pad0[0x18];
    void *kind_18;
} Pot;

extern const char D_80154544[]; /* "PotContents" */
extern unsigned char D_80143094[];

void func_800CA4A4(Stream *stream, const char *tag);
extern void func_800CE918(Pot *list, Stream *stream);
extern s32 func_800AFD08(void *table, void *obj);

void func_800D0064(Pot *pot, Stream *stream) {
    u8 kind;

    func_800CA4A4(stream, D_80154544);
    func_800CE918(pot, stream);
    kind = func_800AFD08(D_80143094, pot->kind_18);
    stream->vtable->write((u8 *)stream + stream->vtable->write_delta, 1, &kind);
}
