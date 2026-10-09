#include "common.h"
typedef struct { signed char x, y; } Pair800459A8;
typedef struct { s32 a, b; } Key800453E4;
/* Complete 0x18-byte receiver: position pointer, kind/padding, two keys. */
typedef struct {
    Pair800459A8 *pos;
    unsigned char kind, padding[3];
    Key800453E4 field08, field10;
} Buf800459A8;
extern Pair800459A8 func_800453E4(Buf800459A8 *, Key800453E4 *);
Pair800459A8 func_800453B8(Buf800459A8 *buf) {
    return func_800453E4(buf, &buf->field08);
}
