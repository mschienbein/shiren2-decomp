#include "func_801EE7B8_1406D8/types.h"

extern D_801F51EC_s *D_801F51EC_14710C;
extern u8 D_801F5CCD_147BED;
s32 func_801E5184_1370A4(void);
u8 func_801E53A4_1372C4(s32);
s32 func_801E51F4_137114(u16 code);
void func_801EE8CC_1407EC(s32);

void func_801EE7B8_1406D8(void) {
    u16 code;

    if (func_801E5184_1370A4()) {
        u8 next;

        D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E++] = D_801F5CCD_147BED;
        next = func_801E53A4_1372C4(D_801F51EC_14710C->x4);
        D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E++] = next;
        /* Pack the two unsigned bytes; their bit ranges do not overlap. */
        code = next;
        code += D_801F5CCD_147BED << 8;
        D_801F51EC_14710C->x4++;
    } else {
        u8 c = D_801F5CCD_147BED;

        D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E++] = c;
        code = c;
    }
    if (D_801F51E8_147108->b999 == 2 && !(u8)func_801E51F4_137114(code)) {
        D_801F51E8_147108->b998 = 1;
    }
    func_801EE8CC_1407EC(1);
}
