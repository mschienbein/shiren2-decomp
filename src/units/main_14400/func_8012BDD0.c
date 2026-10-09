#include "common.h"

typedef unsigned char u8;
typedef float f32;

/* Wave table (ALWaveTable-like); offsets are relocated in place once. */
typedef struct {
    u8 *base;   /* sample offset; top byte 0xFF marks an absolute address */
    s32 len;
    u8 type;    /* 0 = ADPCM */
    u8 flags;   /* nonzero once relocated */
    u8 padA[2];
    void *loop;
    void *book;
} WaveTable;

/* Per-wave tuning: the file stores a signed byte (cents) that becomes a float. */
typedef union {
    u8 raw;
    f32 value;
} Tune;

typedef struct {
    u8 pad00[0x10];
    s32 flags_10;   /* sign bit set once relocated */
    u8 pad14[0x20 - 0x14];
    s32 count_20;
    union {
        struct {
            u8 *semitones;
            Tune *tunes;
            WaveTable **waves;
        } named;
        void *slots[3];
    } tables;
} Bank;

extern void func_8012C300(void **slots, void *base, s32 count);
extern void func_800347A0(void);

/*
 * local-arithmetic-qualification: wave fields hold file-relative offsets until
 * relocated (a top byte of 0xFF marks an absolute address); adding the integer
 * value of the bank or sample base to the stored offset reproduces addu
 * offset,base as in func_8012C300. The relocated stored fields remain pointers.
 */
void func_8012BDD0(void *bank, void *sampleBase) {
    Bank *b = bank;
    s32 i;
    u8 raw;
    s32 n;
    Tune *tune;
    f32 f;

    if (b->flags_10 & 0x80000000) {
        return;
    }
    b->flags_10 |= 0x80000000;
    func_8012C300(b->tables.slots, b, 3);
    func_8012C300((void **)b->tables.named.waves, b, b->count_20);
    for (i = 0; i < b->count_20; i++) {
        tune = &b->tables.named.tunes[i];
        raw = tune->raw;
        if (raw & 0x80) {
            n = raw - 0x100;
        } else {
            n = raw;
        }
        f = n;
        tune->value = f / 100.0;
        raw = b->tables.named.semitones[i] - '0';
        /* ODD_C: convert in each signed-byte arm; their common conversion
         * tail must precede the load of the stored cents adjustment. */
        if (raw & 0x80) {
            n = raw - 0x100;
            f = n;
        } else {
            n = raw;
            f = n;
        }
        tune->value += f;
        if (b->tables.named.waves[i]->flags == 0) {
            u8 *base = b->tables.named.waves[i]->base;

            if (((u32)base & 0xFF000000) != 0xFF000000) {
                base += (s32)sampleBase;
                b->tables.named.waves[i]->base = base;
            }
            b->tables.named.waves[i]->flags = 1;
            if (b->tables.named.waves[i]->loop != 0) {
                b->tables.named.waves[i]->loop = (void *)((s32)b->tables.named.waves[i]->loop + (s32)bank);
            }
            if (b->tables.named.waves[i]->type == 0) {
                b->tables.named.waves[i]->book = (void *)((s32)b->tables.named.waves[i]->book + (s32)bank);
            }
        }
    }
    func_800347A0();
}
