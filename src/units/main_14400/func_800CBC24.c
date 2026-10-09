#include "common.h"

/* Bit reader over a packed buffer: buffer pointer, bit position, size (func_800A09B0). */
typedef struct {
    void *buffer;
    u32 position;
    u32 size;
} BitStream;

/* Unpacked 0x14-byte record (packed by func_800CB884). */
typedef struct {
    unsigned char f0, f1, f2, f3;
    unsigned char f4[4];
    unsigned char f8, f9, fA;
    u32 fC;
    u32 f10;
} Record;

extern void *func_800A09B0(BitStream *stream, void *buffer, u32 size);
extern void func_800A0B74(void *stream, unsigned char *output, u32 bit_count);
extern void func_800A0BB0(void *stream, unsigned char *output, s32 byte_count);
extern void func_800A0ACC(BitStream *stream, u32 *output, u32 bit_count);

void func_800CBC24(void *packet, Record *record) {
    BitStream reader;
    func_800A09B0(&reader, packet, 0x14);
    func_800A0B74(&reader, &record->f0, 5);
    func_800A0B74(&reader, &record->f1, 7);
    func_800A0B74(&reader, &record->f2, 7);
    func_800A0B74(&reader, &record->f3, 7);
    func_800A0BB0(&reader, record->f4, 4);
    func_800A0B74(&reader, &record->f8, 7);
    func_800A0B74(&reader, &record->f9, 8);
    func_800A0B74(&reader, &record->fA, 7);
    func_800A0ACC(&reader, &record->fC, 0x1E);
    func_800A0ACC(&reader, &record->f10, 0x1B);
}
