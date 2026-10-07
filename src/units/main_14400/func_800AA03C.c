#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 index; u8 count; } Cursor800AA03C;
/* 0x18-byte resource record of the 21-record table at 0x80142B1C: five pointer-valued
 * words, then four metadata bytes at +0x14 (0x80142B30 is record 0's metadata). */
typedef struct { void *words[5]; u8 metadata[4]; } Record800AA03C;
extern u8 D_80142F1B;
extern Cursor800AA03C D_80142F24;
extern Record800AA03C D_80142B1C[];
s32 func_800AA03C(void) {
    u8 flags = D_80142F1B;
    if (flags & 8) {
        return 0;
    }
    switch (flags & 3) {
    case 1:
        return D_80142F24.count < D_80142B1C[D_80142F24.index].metadata[0] - 1;
    case 2:
        return D_80142F24.count != 0;
    }
    return 0;
}
