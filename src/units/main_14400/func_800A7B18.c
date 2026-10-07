#include "common.h"
typedef short s16;
typedef struct { char data[0x18]; } Buf800A7B18;
void func_80136910(Buf800A7B18 *obj, void *a, u32 c, u32 b, u32 e);
void func_800A7ADC(void *, Buf800A7B18 *);
void func_800A7B18(void *target, void *source, s32 amount, s32 kind) {
    Buf800A7B18 buf;
    Buf800A7B18 *p = &buf;
    func_80136910(p, source, (s16)amount, kind, 8);
    func_800A7ADC(target, p);
}
