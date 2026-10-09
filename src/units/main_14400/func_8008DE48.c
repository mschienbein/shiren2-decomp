#include "common.h"
/* The range is a cartridge device-address interval, not a CPU pointer. */
extern s32 func_8008DD1C(void *self, u32 devAddr, s32 size);
s32 func_8008DE48(void *self, u32 start, u32 end) {
    return func_8008DD1C(self, start, end - start);
}
