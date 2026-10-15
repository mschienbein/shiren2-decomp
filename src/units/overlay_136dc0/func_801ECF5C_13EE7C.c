#include "func_801ECF5C_13EE7C/types.h"

s32 func_801E81F4_13A114(s32 index, const char *text);
void *func_80032D94(void *dst, const void *src, u32 size);
void func_801EE8CC_1407EC(s32);

s32 func_801ECF5C_13EE7C(const char *text, s32 index) {
    s32 start = func_801E81F4_13A114(index, text);
    s32 end = func_801E81F4_13A114(index + 1, text);
    s32 n;
    u32 destination;

    if (start < end) {
        n = end - start;
        /* This target's o32 pointers and u32 addresses are both 32 bits.
         * Accumulate the byte offset before converting back to a pointer. */
        destination = D_801F51E8_147108->len_11E;
        destination += (u32)D_801F51E8_147108;
        /* Byte addressing stays within the full 0xA04-byte state object;
         * the text buffer starts at 0x126 and ends at the 0x2A6 stack. */
        func_80032D94((u8 *)destination + 0x126, text + start, n);
        D_801F51E8_147108->len_11E += n;
        D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E] = 0;
        func_801EE8CC_1407EC(0);
        return 1;
    }
    return 0;
}
