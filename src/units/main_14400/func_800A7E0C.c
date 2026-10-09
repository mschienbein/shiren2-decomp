#include "common.h"
/* func_800A65E4 writes the direction result into out_direction and returns it;
 * the caller's 8-byte temporary is then handed to func_800A6690. */
extern void *func_800A65E4(void *out_direction, void *object, void *target);
extern void func_800A6690(void *object, s32 *pair, s32 value);
void func_800A7E0C(void *object, void *target, s32 mode) {
    s32 pair[2];
    s32 *pair_ptr = pair;
    func_800A65E4(pair_ptr, object, target);
    func_800A6690(object, pair_ptr, mode);
}
