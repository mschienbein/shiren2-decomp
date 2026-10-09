#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u16 first; u16 offset; } Range;
typedef struct { char text[0xF8]; } Buffer;
typedef struct { s32 offsets[1]; const char text[8]; } FixedText;
extern u8 D_80138CC0;
extern const char D_80138CC1[];
extern const Range D_8014AA94[14];
extern const u32 D_8014D430[];
extern const u8 D_00179560[];
extern Buffer D_80160C20[8];
extern const FixedText D_80151154;
extern void func_8006AAF0(void *dst, u32 devAddr, s32 size);

static inline s32 find_range(u32 key)
{
    s32 index = 1;
    for (;;) {
        if (key < D_8014AA94[index].first)
            return index - 1;
        ++index;
    }
}

char *func_80048480(u16 id)
{
    u32 key = id;
    s32 index;
    s32 slot;
    u8 next;
    u32 address;
    u32 aligned;
    s32 skip;
    Buffer *buffer;
    if (key == 0)
        return (char *)D_80138CC1;
    if (key < 0x5DC1U) {
        index = find_range(key);
        id = D_8014AA94[index].offset + (id - D_8014AA94[index].first);
        slot = D_80138CC0;
        next = slot + 1;
        D_80138CC0 = next;
        /* local-arithmetic-qualification: the linker symbol names a cartridge
         * ROM address; DMA requires an aligned numeric device address. */
        address = D_8014D430[id] + (u32)D_00179560;
        aligned = address & ~7U;
        skip = address - aligned;
        buffer = &D_80160C20[slot];
        if (next >= 8U)
            D_80138CC0 = 0;
        func_8006AAF0(buffer, aligned, 0xF8);
        buffer->text[0xF7] = 0;
        return buffer->text + skip;
    }
    return (char *)D_80151154.text + D_80151154.offsets[key - 0x5DC1];
}
