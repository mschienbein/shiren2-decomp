#include "common.h"

/* +0x40 is the flags halfword written by func_800E10E0; read its low-byte nibble. */
typedef struct {
    unsigned char unknown_00[0x40];
    unsigned short field_40;
} Func800E110CView;

u32 func_800E110C(const Func800E110CView *arg0)
{
    return (unsigned char)arg0->field_40 >> 4;
}
