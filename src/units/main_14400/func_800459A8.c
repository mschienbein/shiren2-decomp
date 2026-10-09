#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s8 x; s8 y; } Pair800459A8;
typedef struct { s32 a, b; } Key800453E4;
/* Complete receiver copied by the original constructor at +8 and +0x10. */
typedef struct {
    Pair800459A8 *pos;
    u8 kind, padding[3];
    Key800453E4 field08, field10;
} Buf800459A8;
extern Pair800459A8 D_8014BF98;
/* Assembly-only constructor: the escaped parameter lifetime remains unresolved. */
void func_80045370(Buf800459A8 *buf, Key800453E4 *key, Pair800459A8 pos);
Pair800459A8 func_800453B8(Buf800459A8 *buf);
void func_80045948(s32 id, Pair800459A8 pos);
void func_800459A8(s32 id, Key800453E4 *key) {
    Buf800459A8 buf;
    Pair800459A8 pos;
    pos = D_8014BF98;
    func_80045370(&buf, key, pos);
    pos = func_800453B8(&buf);
    func_80045948(id, pos);
}
