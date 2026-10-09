#include "common.h"

typedef struct {
    unsigned char pad_00[0x1E];
    unsigned char upper:5;
    unsigned char enabled:1;
    unsigned char lower:2;
} Target;
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern void func_80049A04(unsigned short id, ...);

/* The forwarding callers (e.g. func_8011BF3C) supply the receiver in a0; this
 * handler only inspects the target, so self is intentionally unused. */
void func_801130C0(void *self, void *target)
{
    unsigned char enabled = ((Target *)target)->enabled;
    if (enabled) {
        func_80049CB4(0x132);
        func_800498E4(0x223);
        func_80049A04(0xDD);
    }
}
