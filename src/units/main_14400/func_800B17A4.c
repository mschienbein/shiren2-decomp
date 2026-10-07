#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad[0x14]; } Entry;
extern Entry D_801431F0[];
extern u16 D_8014344A;
extern u8 D_8014344C;
s32 func_800A3214(Entry *entry);
void func_800B17A4(void) {
    s32 i;
    D_8014344A = 0;
    for (i = 0; i < D_8014344C; i++) {
        D_8014344A += func_800A3214(&D_801431F0[i]);
    }
}
