#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_00[0x40]; u16 flags_40; } Flags800E1118;
/* The sole original caller (func_800E11C0, 0x800E1840) passes its int kind un-narrowed:
 * full-width parameter, narrowed to a byte here (0x800E1118). */
void func_800E1118(Flags800E1118 *object, s32 value)
{
    object->flags_40 = (object->flags_40 & 0xF0FF) | (((u8)value - 0x11) << 8);
}
