#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Static 8-byte record buffer (.bss 0x80160B48..0x80160B4F); func_8006AC30 loads one
 * 4-byte {base, count} range entry into its start. */
typedef struct { u16 base; u8 count; u8 pad3; u8 pad4[4]; } RangeBuffer;
extern RangeBuffer D_80160B48;
extern u8 D_00194FC0[];
extern u8 D_2025440[];
s32 func_800D7ED0(s32 id);
void func_8006AC30(void *dst, void *romStart, void *romEnd, s32 size, s32 index, s32 count);

/* Maps an id to its (group, index-within-group) pair by scanning the 58 range entries. */
s32 func_8004505C(s32 id, u8 *outGroup, u8 *outIndex)
{
    s32 i;

    id = func_800D7ED0(id);
    i = 0;
    if ((u32)id < 0x108) {
        while (i < 58) {
            func_8006AC30(&D_80160B48, D_00194FC0, D_2025440, 4, i, 1);
            if (id < D_80160B48.base + D_80160B48.count) {
                *outGroup = i + 29;
                *outIndex = id - D_80160B48.base + 1;
                return 1;
            }
            i++;
        }
    }
    return 0;
}
