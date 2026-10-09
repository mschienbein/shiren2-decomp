#include "common.h"

extern void func_800C2810(void *);
extern int func_800C28EC(void *iterator);
extern void *func_800C28FC(void *out, void *iterator);
/* Both results are intentionally ignored; the eight-byte output buffer is complete. */
void func_800C2B84(void *state) { s32 value[2]; func_800C2810(state); func_800C28EC(state); func_800C28FC(value, state); }
