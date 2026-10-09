#include "common.h"

/* g++ 2.x member-function descriptor: virtual index or typed direct function. */
typedef struct {
    signed short delta, index;
    union { void (*pfn)(void *); signed short delta2; } u;
} MemberFn;
/* Display +0x100 and callback +0x110 each occupy 0x10 bytes. */
typedef struct { unsigned char pad00[0x100]; unsigned char display100[0x10]; unsigned char callback110[0x10]; } Object;
extern const MemberFn D_80153070;
extern unsigned char D_8014AB98[];
extern void func_80095E58(void *, void *, MemberFn);
extern void func_800486A4(void *, void *, void *);
void func_8009F9E0(Object *state) { void *part = state->callback110; func_80095E58(part, state, D_80153070); func_800486A4(state->display100, D_8014AB98, part); }
