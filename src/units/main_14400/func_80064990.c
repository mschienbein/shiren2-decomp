#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 unk0;
    u8 unk1;
} Pair80064990;

extern Pair80064990 D_8016AB04[][54];

u8 func_80064990(s32 row, s32 col) {
    return D_8016AB04[row][col].unk0;
}
