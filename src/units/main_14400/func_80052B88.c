/* Independent reconstruction of main_14400 ROM 0x25BC8..0x25BF0.
 * The original tracks a signed word count and a separate byte displacement.
 * Four 32-bit opaque slots remain externally owned BSS.
 */
#include "common.h"
typedef unsigned char u8;

typedef char word_width[(sizeof(u32) == 4) ? 1 : -1];
typedef char index_width[(sizeof(s32) == 4) ? 1 : -1];
typedef char pointer_width[(sizeof(void *) == 4) ? 1 : -1];

extern u32 D_80161674[4];

void func_80052B88(void)
{
    s32 index;
    s32 byte_offset;

    index = 3;
    byte_offset = 12;
    do {
        *(u32 *)((u8 *)D_80161674 + byte_offset) = 0;
        index--;
        byte_offset -= sizeof(u32);
    } while (index >= 0);
}
