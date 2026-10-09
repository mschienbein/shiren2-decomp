#include "common.h"
typedef unsigned char u8;
typedef struct { u8 *field_0; u8 *field_4; } Span;
typedef struct { u8 pad_0[0x1C]; Span *field_1C; } Obj80094DAC;
extern Obj80094DAC *func_800C9E10(void);
s32 func_80042B10(void)
{
    Span *span = func_800C9E10()->field_1C;
    return span->field_4 - span->field_0;
}
