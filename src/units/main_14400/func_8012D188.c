#include "common.h"

typedef unsigned char u8;

/* Audio DMA cache line: list links, ROM base and its RAM copy. */
typedef struct DmaLine {
    struct DmaLine *prev;
    struct DmaLine *next;
    s32 age;
    u32 rom_base;
    u8 *buffer;
} DmaLine;

DmaLine *func_8012D1F4(u32 address, s32 length);
u32 func_800340F0(void *addr);

/* DMA callback (see func_8012D178): map a ROM address to a cached RAM copy.
 * `state` is part of the callback contract but unused here. */
u32 func_8012D188(u32 address, s32 length, void *state) {
    DmaLine *line = func_8012D1F4(address, length);

    if (line == 0) {
        return func_800340F0((void *)address);
    }
    if ((address & 0xFF000000) == 0xFF000000) {
        address &= 0xFFFFFF;
        address += 0x140000;
    }
    /* local-arithmetic-qualification: the bounded buffer + (address - rom_base)
     * form reverses the original load/add/sub sequence; perform only this
     * cache-offset translation as integer arithmetic, then restore a pointer. */
    return func_800340F0((void *)((u32)line->buffer + address - line->rom_base));
}
