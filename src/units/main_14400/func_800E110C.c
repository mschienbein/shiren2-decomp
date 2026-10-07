#include "common.h"

/* Partial view of the accessed byte; the owner type and full size are unknown. */
typedef struct {
    unsigned char unknown_00[0x41];
    unsigned char byte_41;
} Func800E110CView;

u32 func_800E110C(const Func800E110CView *arg0)
{
    return arg0->byte_41 >> 4;
}
