#include "common.h"
typedef unsigned char u8;
extern void func_800AA700(u8 *first, u8 *second, u8 *excluded);
extern void *func_800A8694(u8 kind, u8 variant, void *memory);

/* Pick a random kind/variant pair (no exclusion list) and construct it. */
void *func_800AA63C(void) {
    u8 kind, variant;
    func_800AA700(&kind, &variant, 0);
    if (kind == 0) return 0;
    return func_800A8694(kind, variant, 0);
}
