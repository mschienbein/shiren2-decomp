#include "common.h"
typedef struct { void *f0; u32 f4; u32 f8; s32 fC; } BitStream;
void *func_800A09B0(BitStream *bs, void *buf, u32 size);
void func_800A09C4(BitStream *bs, void *buf, u32 size);
void func_800A09E8(BitStream *bs, u32 value, u32 bits);
static inline void BitStream_rewind(BitStream *bs) {
    func_800A09C4(bs, bs->f0, bs->f8);
}
typedef struct {
    unsigned char f0, f1, f2, f3;
    unsigned char f4[4];
    unsigned char f8, f9, fA;
    s32 fC;
    s32 f10;
} Record;
void func_800A0A64(BitStream *bs, void *data, s32 count);
void func_800CB884(Record *r, void *buf) {
    BitStream bs;
    func_800A09B0(&bs, buf, 0x14);
    BitStream_rewind(&bs);
    func_800A09E8(&bs, r->f0, 5);
    func_800A09E8(&bs, r->f1, 7);
    func_800A09E8(&bs, r->f2, 7);
    func_800A09E8(&bs, r->f3, 7);
    func_800A0A64(&bs, r->f4, 4);
    func_800A09E8(&bs, r->f8, 7);
    func_800A09E8(&bs, r->f9, 8);
    func_800A09E8(&bs, r->fA, 7);
    func_800A09E8(&bs, r->fC, 30);
    func_800A09E8(&bs, r->f10, 27);
}
