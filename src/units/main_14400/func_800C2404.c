#include "common.h"

/* func_800430C4 reads the incoming object's fields and virtual table. */
extern void func_800430C4(void *object);
extern s32 func_80049CB4(s32 id, ...);

/* Slot +0x0C of mode vtable D_80153ED8 (0x80153EE4). Its dispatcher func_800AA9FC calls the
 * slot with the adjusted receiver at 0x800AAB88..0x800AAB94 and discards v0 (0x800AAB9C), as
 * do the base/sibling targets func_80042C20, func_80043228 and func_800C246C: void(self). */
void func_800C2404(void *object)
{
    func_800430C4(object);
    func_80049CB4(5, 1);
}
