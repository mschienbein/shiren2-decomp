#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 data[20]; } Entry800B21BC;
extern u8 D_8014344C;
extern u8 D_80147620[];
extern Entry800B21BC D_801431F0[];
u16 func_800C58DC(void *table, u16 index);

Entry800B21BC *func_800B21BC(void) {
    if (D_8014344C == 0) {
        return 0;
    }
    return &D_801431F0[func_800C58DC(D_80147620, D_8014344C - 1)];
}
