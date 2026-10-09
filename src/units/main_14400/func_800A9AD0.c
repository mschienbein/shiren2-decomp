#include "common.h"

typedef union { unsigned short packed; struct { unsigned char first; unsigned char chance; } bytes; } Pair;
typedef struct { Pair entries[5]; } Probabilities;
extern s32 D_80142D14;
extern const Probabilities D_80156A94;
extern Probabilities D_801C51F0;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

extern unsigned char D_80147620[];
extern s32 func_800C587C(void *rng, unsigned char limit);

/* ODD_C: whole-table accessor preserves independent element load expressions. */
static inline unsigned short probability(const Probabilities *table, s32 i) {
    return table->entries[i].packed;
}

void func_800A9AD0(void)
{
    s32 i;
    if (!D_80142D14) {
        unsigned short a = probability(&D_80156A94, 0);
        unsigned short b = probability(&D_80156A94, 1);
        unsigned short c = probability(&D_80156A94, 2);
        unsigned short d = probability(&D_80156A94, 3);
        unsigned short e = probability(&D_80156A94, 4);
        D_80142D14 = 1;
        D_801C51F0.entries[0].packed = a;
        D_801C51F0.entries[1].packed = b;
        D_801C51F0.entries[2].packed = c;
        D_801C51F0.entries[3].packed = d;
        D_801C51F0.entries[4].packed = e;
    }
    D_80142F24.masks[1] = 0;
    for (i = 4; i != -1; i--) {
        if (func_800C587C(D_80147620, D_801C51F0.entries[i].bytes.chance)) {
            D_80142F24.masks[1] |= 1 << i;
        }
    }
}
