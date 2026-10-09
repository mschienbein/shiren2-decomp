#include "common.h"

extern unsigned short func_800C58DC(void *, unsigned short);
u32 func_800C5AB4(void *state, unsigned short start, unsigned short end) { unsigned short width = end - start; unsigned short first = func_800C58DC(state, width); unsigned short second = func_800C58DC(state, width); return start + (((u32)first + second) >> 1); }
