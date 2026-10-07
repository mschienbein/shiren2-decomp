#include "common.h"
typedef struct { char pad[0xE]; unsigned char xE; char pad2; s32 x10; } S;
typedef struct { unsigned char v; } Byte;
static inline s32 byte_bit(Byte *b) { return (b->v + 4) % 8; }
s32 func_801290F4(S *p, Byte b, s32 flag) {
    if ((p->x10 == 1 && flag != 0) || (p->x10 == 2 && flag == 0)) return 0;
    return (p->xE >> byte_bit(&b)) & 1;
}
