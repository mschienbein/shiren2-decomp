#include "common.h"
typedef unsigned char u8;
typedef struct {
    u8 pad_00[0x18]; short adjustment_18; unsigned short pad_1A;
    void (*write_1C)(void *, s32, const void *);
} StreamVTable800EF7AC;
typedef struct { u8 pad_00[0x18]; StreamVTable800EF7AC *vtable_18; } Stream800EF7AC;
extern const char D_80159244[];
extern void func_800CA4A4(void *stream, void *name);
extern s32 func_800EF74C(u8 id);
extern void *func_800EF72C(u8 id);
extern s32 func_800EF78C(u8 id);
extern void *func_800EF76C(u8 id);
/* The buffer getters load pointer fields of the descriptors at D_80159250. */
void func_800EF7AC(Stream800EF7AC *stream) {
    s32 id;
    for (func_800CA4A4(stream, (void *)D_80159244), id = 0x18; id < 0x1D; id++) {
        stream->vtable_18->write_1C((u8 *)stream + stream->vtable_18->adjustment_18, func_800EF74C((u8)id), func_800EF72C((u8)id));
        stream->vtable_18->write_1C((u8 *)stream + stream->vtable_18->adjustment_18, func_800EF78C((u8)id), func_800EF76C((u8)id));
    }
}
