#include "common.h"
typedef unsigned char u8;
typedef struct { void *buffer_0; s32 size_4; u8 *device_8; void *cursor_C; s32 remaining_10; } Stream80083B10;
extern void func_8006AAF0(void *dst, u32 devAddr, s32 size);
void func_80083960(Stream80083B10 *stream, u8 *src, void *table, s32 count) {
    /* The DMA API encodes the stream source as a PI device address. */
    func_8006AAF0(table,(u32)src,count); stream->device_8=src; stream->buffer_0=table; stream->size_4=count; stream->remaining_10=count; stream->cursor_C=table;
}
