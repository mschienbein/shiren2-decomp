#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { u8 first[5]; u8 next[4]; } MuseumData;
typedef struct { u8 pad0[0x18]; s16 adjust18; s16 pad1A; void (*transfer1C)(void *, s32, void *); } Methods;
typedef struct { u8 pad0[0x18]; const Methods *methods18; } Stream;
extern const char D_80153210[];
extern void func_800CA4A4(void *, void *);
void func_800A176C(void *a, void *b) {
    MuseumData *data = a;
    Stream *stream = b;
    func_800CA4A4(stream, (void *)D_80153210);
    stream->methods18->transfer1C((u8 *)stream + stream->methods18->adjust18, 5, data);
    stream->methods18->transfer1C((u8 *)stream + stream->methods18->adjust18, 4, data->next);
}
