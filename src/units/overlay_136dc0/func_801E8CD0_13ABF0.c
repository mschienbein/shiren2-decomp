#include "func_801E8CD0_13ABF0/types.h"
s32 func_801E8CD0_13ABF0(void *self) {
    S06Widget *widget = self;
    if (widget->field5C == 1) {
        widget->field64 = -1;
        return 0;
    }
    return 2;
}
