#include "common.h"
/* D_80154550 (item-set family D_80154300) slot +0x1C is func_800CE6F8: void (void *self, s32 value). */
typedef struct {
    unsigned char pad00[0x18]; short adjustment; unsigned short pad1A;
    void (*set)(void *self, s32 value);
} Vtable;
typedef struct { void *storage; Vtable *vtable; } Descriptor;
typedef struct Rng Rng;
extern Rng D_80147620;
extern const unsigned char D_80157778[];
extern unsigned char func_800AE98C(unsigned char *item);
extern void *func_8011422C(unsigned char *item);
extern s32 func_800C5844(void *rng, unsigned char low, unsigned char high);
void func_801141B0(unsigned char *item) {
    unsigned char limits = D_80157778[func_800AE98C(item)];
    s32 low = limits & 15;
    s32 high = limits >> 4;
    Descriptor *descriptor = func_8011422C(item);
    unsigned char value = func_800C5844(&D_80147620, low, high);
    descriptor->vtable->set((unsigned char *)descriptor + descriptor->vtable->adjustment, value);
}
