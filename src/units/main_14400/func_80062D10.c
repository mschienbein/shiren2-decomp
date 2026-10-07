#include "common.h"

typedef struct Node Node;
typedef struct Sub { s32 x0; void *x4; void *x8; void *xC; char pad[0xC]; void *x1C; s32 x20; void *x24; } Sub;
struct Node { void *x0; void *x4; char pad[0x28]; Sub *x30; };
/* Relocates one encoded segment word: unchanged unless its 4-bit tag matches. */
void *func_80062CCC(void *value, void *base, u32 tag);
void func_80062D10(Node **head, void *base, unsigned char seg) {
    Node **list = head;
    u32 tag = ((u32)seg << 24) & 0x0F000000;
    Node *n;
    for (; (n = *list) != 0; list++) {
        if (((u32)n & 0x0F000000) == tag) {
            n = func_80062CCC(n, base, seg);
            *list = n;
            n->x0 = func_80062CCC(n->x0, base, seg);
            n->x4 = func_80062CCC(n->x4, base, seg);
            if (n->x30 != 0) {
                n->x30 = func_80062CCC(n->x30, base, seg);
                if (n->x30->x4 != 0) n->x30->x4 = func_80062CCC(n->x30->x4, base, seg);
                if (n->x30->x8 != 0) n->x30->x8 = func_80062CCC(n->x30->x8, base, seg);
                if (n->x30->xC != 0) n->x30->xC = func_80062CCC(n->x30->xC, base, seg);
                if (n->x30->x1C != 0) n->x30->x1C = func_80062CCC(n->x30->x1C, base, seg);
                if (n->x30->x24 != 0) n->x30->x24 = func_80062CCC(n->x30->x24, base, seg);
            }
        }
    }
}
