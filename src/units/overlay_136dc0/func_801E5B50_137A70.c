#include "func_801E5B50_137A70/types.h"

void func_801E58A0_1377C0(void);
void func_801E5A3C_13795C(void);

void func_801E5B50_137A70(s32 index, u8 key, u16 *first, u16 *second) {
    D_801F66E0_view *cursor = &D_801F66E0_148600[index + 1];

    D_801F6710_148630 = cursor;
    cursor->x2 = index;
    cursor->x4 = 0;
    cursor->x0 = key;
    func_801E58A0_1377C0();
    if (D_801F6710_148630->x8 != D_801F6710_148630->xA) {
        func_801E5A3C_13795C();
    }
    *first = D_801F6710_148630->x8;
    *second = D_801F6710_148630->xA;
}
