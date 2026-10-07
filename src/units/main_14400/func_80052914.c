#include "common.h"

/* Eight-byte ROM range record: numeric ROM start/end offsets, not CPU pointers. */
typedef struct { u32 start; u32 end; } Range;
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
void func_80052914(s32 index, void *dst, Range *ranges) {
    ranges += index;
    func_8006AAF0(dst, ranges->start, ranges->end - ranges->start);
}
