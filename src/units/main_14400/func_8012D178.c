#include "common.h"
typedef u32 (*Func8012DmaProc)(u32 address, s32 length, void *state);
u32 func_8012D188(u32 address, s32 length, void *state);
Func8012DmaProc func_8012D178(void **state) { (void)state; /* unused state slot */ return func_8012D188; }
