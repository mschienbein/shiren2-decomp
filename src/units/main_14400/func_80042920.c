#include "common.h"
/* D_80138AE0 is one 0x20-byte reader object (vtable at +0, constructed by func_80042BFC);
 * func_80048728 receives its embedded 0x10-byte task at +4 (handle word at +0x10). */
typedef struct { s32 x; s32 y; void *target; s32 handle; } Task80042920;
typedef struct { void *vtable; Task80042920 task_04; unsigned char pad_14[0xC]; } Reader80042920;
extern Reader80042920 D_80138AE0;
extern void func_80048728(void *queue);
void func_80042920(void) { func_80048728(&D_80138AE0.task_04); }
