#include "common.h"
typedef struct { unsigned char v; } Byte;
/* Partial view of the 0x20-byte work object constructed by func_800C2B40. */
typedef struct { char pad[0x18]; void *x18; char pad2[4]; } Buf;
Buf *func_800C2B40(Buf *buf, void *src, Byte *b, s32 n);
s32 func_800C2BBC(Buf *buf, unsigned char k);
void *func_800B371C(void *src, Byte b, s32 n, s32 k) {
    Buf buf;
    func_800C2B40(&buf, src, &b, n + 1);
    if (func_800C2BBC(&buf, k) == 0) return 0;
    return buf.x18;
}
