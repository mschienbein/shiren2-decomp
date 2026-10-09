#include "common.h"

/* Opaque view of the 405-entry sprite-descriptor pointer table (modelled in func_80071CCC). */
typedef struct SpriteDesc SpriteDesc;
extern SpriteDesc *D_801D25A0[];

/* Clear every descriptor pointer, last entry first. */
void func_80071060(void) {
    s32 index = 0x194;
    s32 offset = 0x650;
    do {
        *(SpriteDesc **)((unsigned char *)D_801D25A0 + offset) = 0;
        index--;
        offset -= sizeof(SpriteDesc *);
    } while (index >= 0);
}
