#include "common.h"

typedef struct { s32 a; s32 b; } Pair;
typedef struct { unsigned char pad[0x5C]; void *f5C; } Node;
/* Each menu record is three words: packed message selector, value, and result.
 * Own both two-record arrays at their real bases, 0x801424A0 and 0x801424B8.
 * The splat labels D_801424A4 / D_801424BC identify word 1, not scalar objects. */
typedef s32 MenuRecord[3];
MenuRecord D_801424A0[2] = {{0x045B0000, 0, 0}, {0x045E0000, 0, 2}};
MenuRecord D_801424B8[2] = {{0x045C0000, 0, 0}, {0x02960000, 0, 3}};
extern Pair D_80138F78;
extern Pair D_80138F80;
s32 func_800D15B0(void *board);
void func_80097240(Node *n, MenuRecord *records, Pair *pairs, s32 *counts);
void func_8009D7EC(Node *n, void *board, Pair *src, s32 value) {
    Pair pairs[2];
    s32 counts[2];
    n->f5C = board;
    pairs[1].a = src->a;
    pairs[1].b = src->b;
    counts[1] = 1;
    if (func_800D15B0(n->f5C)) {
        s32 *slot = &D_801424B8[0][1];
        *slot = value;
        pairs[0].a = D_80138F78.a;
        pairs[0].b = D_80138F78.b;
        counts[0] = 2;
        func_80097240(n, (MenuRecord *)(slot - 1), pairs, counts);
    } else {
        s32 *slot = &D_801424A0[0][1];
        *slot = value;
        pairs[0].a = D_80138F80.a;
        pairs[0].b = D_80138F80.b;
        counts[0] = 2;
        func_80097240(n, (MenuRecord *)(slot - 1), pairs, counts);
    }
}
