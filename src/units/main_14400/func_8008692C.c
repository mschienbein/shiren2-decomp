#include "common.h"
typedef struct { unsigned char pad_00[4]; unsigned short field_04; } Obj8008692C;
extern s32 D_801E4E70;
extern s32 func_800419C4(void);
extern s32 func_80042990(void);
extern s32 func_80042B78(s32 mode);
extern s32 func_8005B058(void);
extern void func_8005A670(s32 mode, s32 save, s32 instant);
void func_8008692C(Obj8008692C *self) {
    s32 mode = 0;
    if (!D_801E4E70 || func_800419C4() || func_80042990()) {
        mode = 1;
    }
    mode = func_80042B78(mode);
    if ((u32)func_8005B058() < (u32)mode) {
        func_8005A670(mode, 0, 0);
    }
    self->field_04 = 4;
}
