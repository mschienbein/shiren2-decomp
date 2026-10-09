#include "common.h"

typedef struct { unsigned char pad00[2]; char name02[4]; } Obj80094DAC;
extern Obj80094DAC *func_800C9E10(void);
extern char *func_80083F34(void *src, s32 len, char *dst);

/* The virtual method keeps its supplied receiver, but reads the global profile. */
char *func_800ECC38(void *self, char *dst) {
    Obj80094DAC *profile = func_800C9E10();
    *func_80083F34(profile->name02, 4, dst) = 0;
    return dst;
}
