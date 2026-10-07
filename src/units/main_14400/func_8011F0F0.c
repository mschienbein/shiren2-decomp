#include "common.h"
typedef struct {
    void *source;
    u32 kind;
    u32 effect;
    unsigned short amount;
    unsigned short flags;
    unsigned char category;
} Payload;
extern u32 D_8013960C;
extern unsigned short D_801569FA;
extern void func_80136910(void *, void *, u32, u32, u32), func_800A7ADC(void *, void *);
/* Trap apply slot +0x54 supplies five pointers; self, direction and attacker
 * are unused by this override. */
void func_8011F0F0(void *self, void *b, void *c, void *direction, void *attacker) { Payload local; Payload *p = &local; D_8013960C <<= 1; func_80136910(p, b, (short)-D_801569FA, 6, 8); func_800A7ADC(c, p); D_8013960C >>= 1; }
