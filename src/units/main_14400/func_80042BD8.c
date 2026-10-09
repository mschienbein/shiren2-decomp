#include "common.h"
typedef struct { void *vtable; unsigned char pad_04[0xC]; s32 field_10; unsigned char pad_14[0xC]; } Reader;
extern Reader D_80138AE0;
void func_80095010(Reader *reader, s32 mode);
void func_80042BD8(void)
{
    func_80095010(&D_80138AE0, 2);
}
